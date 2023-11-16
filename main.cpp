#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageSelector.h>
#include "DGtal/io/readers/PGMReader.h"
#include "DGtal/io/writers/GenericWriter.h"
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/ColorBrightnessColorMap.h>
#include <DGtal/topology/SurfelAdjacency.h>
#include <DGtal/topology/helpers/Surfaces.h>
#include "DGtal/io/Color.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

template <class T>
void sendToBoard(Board2D &board, T &p_Object, DGtal::Color p_Color);

template <class T>
Curve getBoundary(T &object)
{
    // Khalimsky space
    KSpace kSpace;
    // we need to add a margine to prevent situations such that an object touch the bourder of the domain
    kSpace.init(object.domain().lowerBound() - Point(),
                object.domain().upperBound(), true);

    // 1) Call Surfaces::findABel() to find a cell which belongs to the border
    auto set2d = object.pointSet();
    auto aCell = Surfaces<Z2i::KSpace>::findABel(kSpace, set2d, 10000);

    std::vector<Z2i::Point> boundaryPoints; // boundary points are going to be stored here

    // 2) Call Surfece::track2DBoundaryPoints to extract the boundary of the object
    SurfelAdjacency<KSpace::dimension> SAdj(true);
    Surfaces<KSpace>::track2DBoundaryPoints(boundaryPoints, kSpace, SAdj, set2d, aCell);

    Board2D aBoard;

    GradientColorMap<int> cmap_grad(0, boundaryPoints.size());
    cmap_grad.addColor(Color(50, 50, 255));
    cmap_grad.addColor(Color(255, 0, 0));
    cmap_grad.addColor(Color(255, 255, 10));

    unsigned int d = 0;
    std::vector<Z2i::SCell>::iterator it;
    for (auto it = boundaryPoints.begin(); it != boundaryPoints.end(); it++)
    {
        sendToBoard(aBoard, *it, Color::Red);
        d++;
    }
    aBoard.saveSVG("Basmout.svg", 200, 200, 1);

    // 3) Create a curve from a vector
    Curve boundaryCurve;
    boundaryCurve.initFromSCellsVector(boundaryPoints.at(1));
    return boundaryCurve;
}

template <class T>
void sendToBoard(Board2D &board, T &p_Object, DGtal::Color p_Color)
{
    board << CustomStyle(p_Object.className(), new DGtal::CustomFillColor(p_Color));
    board << p_Object;
}

int main(int argc, char **argv)
{
    setlocale(LC_NUMERIC, "us_US");                                            // To prevent French local settings
    typedef ImageSelector<Domain, unsigned char>::Type Image;                  // type of image
    typedef DigitalSetSelector<Domain, BIG_DS + HIGH_BEL_DS>::Type DigitalSet; // Digital set type
    typedef Object<DT4_8, DigitalSet> ObjectType;                              // Digital object type

    // read an image
    Image japSeg = PGMReader<Image>::importPGM("../RiceGrains/Rice_japonais_seg_bin.pgm");
    Image camargueSeg = PGMReader<Image>::importPGM("../RiceGrains/Rice_camargue_seg_bin.pgm");
    Image basmatiSeg = PGMReader<Image>::importPGM("../RiceGrains/Rice_basmati_seg_bin.pgm");

    // 1) make a "digital set" of proper size
    Image img = basmatiSeg;
    Z2i::DigitalSet set2d(img.domain());

    // 2) populate a digital set from the image using SetFromImage::append()
    SetFromImage<Z2i::DigitalSet>::append<Image>(set2d, img, 1, 255);

    // 3) Create a digital object from the digital set
    std::vector<ObjectType> objects;                                      // All connected components are going to be stored in it
    std::back_insert_iterator<std::vector<ObjectType>> inserter(objects); // Iterator used to populated "objects".

    ObjectType digitalObj(dt4_8, set2d);

    // 4) Set the adjacency pair and obtain the connected components using "writeComponents"
    unsigned int nbc = digitalObj.writeComponents(inserter);

    std::cout << " number of components : " << objects.size() << endl; // Right now size of "objects" is the number of conected components

    // Step 4
    // types definition
    typedef std::vector<Point> Range;
    typedef Range::const_iterator ConstIterator;
    typedef StandardDSS4Computer<ConstIterator> SegmentComputer;
    typedef GreedySegmentation<SegmentComputer> Segmentation;

    auto curveFromBoundary = getBoundary<ObjectType>(digitalObj);

    // Segmentation
    SegmentComputer recognitionAlgorithm;
    auto curveRange = curveFromBoundary.();
    Segmentation theSegmentation(curveFromBoundary.begin(), curveFromBoundary.end(), recognitionAlgorithm);

    Segmentation::SegmentComputerIterator i = theSegmentation.begin();
    Segmentation::SegmentComputerIterator end = theSegmentation.end();
    for (; i != end; ++i)
    {
        SegmentComputer current(*i);
        trace.info() << current << std::endl; // standard output
    }

    return 0;
}
