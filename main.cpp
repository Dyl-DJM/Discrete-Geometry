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

// Must add to have the greedy part to work
#include "DGtal/geometry/curves/GreedySegmentation.h"

#include <string>
#include <math.h>

using namespace std;
using namespace DGtal;
using namespace Z2i;


// STEP 4 and more
typedef FreemanChain<int> Contour4;
typedef ArithmeticalDSSComputer<Contour4::ConstIterator, int, 4> DSS4;
typedef GreedySegmentation<DSS4> Decomposition4;

struct PointTuple{

    PointTuple(int x, int y)
    : _x{x}, _y{y}
    {
    }

    int _x;
    int _y;
};


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
    aBoard.saveSVG("out.svg", 200, 200, 1);

    // 3) Create a curve from a vector
    Curve boundaryCurve;
    std::vector<SCell> contour; // contour

    // tracking and init grid curve
    Surfaces<KSpace>::track2DBoundary(contour, kSpace, SAdj, set2d, aCell);
    boundaryCurve.initFromSCellsVector(contour);
    return boundaryCurve;
}

template <class T>
void sendToBoard(Board2D &board, T &p_Object, DGtal::Color p_Color)
{
    board << CustomStyle(p_Object.className(), new DGtal::CustomFillColor(p_Color));
    board << p_Object;
}




float applyShoelace(const std::vector<PointTuple>& points){

    float sum = 0;
    int x1, y1, x2, y2;

    auto firstPoint = points[0]; 

    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        auto nextPoint = firstPoint;

        if(i < points.size() - 1){
            nextPoint = points[i + 1];
        }
        
        x1 = point._x;
        y1 = point._y;
        x2 = nextPoint._x;
        y2 = nextPoint._y;

        sum += (x1 * y2) - (y1 * x2);
    }

    auto shoelaceRes = 0.5 * abs(sum);
    return shoelaceRes;
}




double euclideanDist(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2 - x1, 2) + (pow(y2 - y1, 2)));
}



double getPerimeter(const std::vector<PointTuple>& points){

    double sum = 0;
    int x1, y1, x2, y2;

    auto firstPoint = points[0]; 

    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        auto nextPoint = firstPoint;

        if(i < points.size() - 1){
            nextPoint = points[i + 1];
        }
        
        x1 = point._x;
        y1 = point._y;
        x2 = nextPoint._x;
        y2 = nextPoint._y;

        sum += euclideanDist(x1, y1, x2, y2);
    }
    return sum;
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
    Image img = japSeg;
    Z2i::DigitalSet set2d(img.domain());

    // 2) populate a digital set from the image using SetFromImage::append()
    SetFromImage<Z2i::DigitalSet>::append<Image>(set2d, img, 1, 255);

    // 3) Create a digital object from the digital set
    std::vector<ObjectType> objects;                                      // All connected components are going to be stored in it
    std::back_insert_iterator<std::vector<ObjectType>> inserter(objects); // Iterator used to populated "objects".

    ObjectType digitalObj(dt4_8, set2d);

    // 4) Set the adjacency pair and obtain the connected components using "writeComponents"
    unsigned int nbc = digitalObj.writeComponents(inserter);

    std::cout << " number of components : " << objects.size() << endl; // Right now size of "objects" is the number of connected components

    // ==================================== Step 4 ======================================

    // types definition
    trace.beginBlock("Greedy");


    // A Freeman chain code is a string composed by the coordinates of the first pixel, and the list of elementary displacements.
    std::stringstream ss(stringstream::in | stringstream::out);

    // Get the boundary cruve
    auto curve = getBoundary(objects[113]);

    // Get the first Point range and put it in the freeman chain
    auto firstPointRange = *(curve.getPointsRange().begin());
    ss << firstPointRange[0] << " " << firstPointRange[1] << " ";

    // Then each code must appear in the freeman chain
    for (auto it = curve.getCodesRange().begin(); it != curve.getCodesRange().end(); it++)
    {
        ss << *it;
    }

    ss << std::endl;

    // Construct the Freeman chain
    Contour4 theContour(ss);

    // == Draw line Version ==

    Board2D aBoardLine;

    // displaying contour
    aBoardLine << SetMode((*(theContour.begin())).className(), "Grid");

    // TODO : À remplacer pour que ça match avec les itérateurs
    for (unsigned int i = 0; i < theContour.size(); i++)
    {
        auto firstP = theContour.getPoint(i);
        auto secondP = theContour.getPoint((i + 1) % theContour.size());
        aBoardLine << firstP;
        aBoardLine.drawLine(firstP[0], firstP[1],
                            secondP[0], secondP[1]);
    }

    // == Other way (got from the example) ==

    // Segmentation
    Decomposition4 theDecomposition(theContour.begin(), theContour.end(), DSS4());

    // Draw each segment
    string styleName = "";
    Board2D aBoard;
    for (Decomposition4::SegmentComputerIterator
             it = theDecomposition.begin(),
             itEnd = theDecomposition.end();
         it != itEnd; ++it)
    {
        aBoard << SetMode("ArithmeticalDSS", "Points")
               << it->primitive();
        aBoard << SetMode("ArithmeticalDSS", "BoundingBox")
               << CustomStyle("ArithmeticalDSS/BoundingBox",
                              new CustomPenColor(Color::Blue))
               << it->primitive();
    }

    aBoard.saveSVG("greedyOut.svg", 600, 600, 10);
    aBoardLine.saveSVG("greedyOutLineVersion.svg", 600, 600, 10);

    // ==================================== Step 5 ======================================

    auto areaGridPoints = objects[113].size();

    std::cout << "Area Version Grid Points : " << areaGridPoints << std::endl;


    // Shoelace Computation
    std::vector<PointTuple> polygonPoints;

    for(auto &segment : theDecomposition){
        auto x = (*(segment.begin()))[0];
        auto y = (*(segment.begin()))[1];
        polygonPoints.push_back(PointTuple(x, y));
    }
    

    auto areaPolygon = applyShoelace(polygonPoints);
    std::cout << "Area Version Polygon Shape (UF) : " << areaPolygon << std::endl;


    // ==================================== Step 6 ======================================

    auto perimeterFromBoundary = curve.size();
    std::cout << "Perimeter : " << perimeterFromBoundary << std::endl;


    auto perimeterFormula = getPerimeter(polygonPoints);
    std::cout << "Perimeter (Polygon) UF: " << perimeterFormula << std::endl;


    return 0;
}
