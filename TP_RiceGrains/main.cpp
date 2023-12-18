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
#include <numeric>
#include <vector>

using namespace std;
using namespace DGtal;
using namespace Z2i;

// STEP 4 and more
typedef FreemanChain<int> Contour4;
typedef ArithmeticalDSSComputer<Contour4::ConstIterator, int, 4> DSS4;
typedef GreedySegmentation<DSS4> Decomposition4;

// Own structure to store points
struct PointTuple
{

    PointTuple(int x, int y)
        : _x{x}, _y{y}
    {
    }

    int _x;
    int _y;
};


// Own structure to store statistic information
struct Stats
{

    // Overloading the << operator
    friend std::ostream& operator<<(std::ostream& os, const Stats& obj) {
        os << "Mean : " << obj._mean << std::endl <<
        "SD : " << obj._sd << std::endl <<
        "Range : " << obj._range <<std::endl;

        return os;
    }

    float _mean;
    float _sd;
    float _range;
};

template <class T>
void sendToBoard(Board2D &board, T &p_Object, DGtal::Color p_Color);

/**
 * Computes the boundary of a digital Object
 */
template <class T>
Curve getBoundary(T &object)
{
    // Khalimsky space
    KSpace kSpace;
    // we need to add a margine to prevent situations such that an object touch the bourder of the domain
    kSpace.init(object.domain().lowerBound() - Point(1, 1),
                object.domain().upperBound() + Point(1, 1), true);

    auto set2d = object.pointSet();

    // 1) Call Surfaces::findABel() to find a cell which belongs to the border
    auto aCell = Surfaces<Z2i::KSpace>::findABel(kSpace, set2d, 100000);

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

    aBoard.saveSVG("Grain.svg", 200, 200, 10);

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

float applyShoelace(const std::vector<PointTuple> &points)
{
    float sum = 0;
    int x1, y1, x2, y2;

    auto firstPoint = points[0];

    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        auto nextPoint = firstPoint;

        if (i < points.size() - 1)
        {
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

double euclideanDist(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + (pow(y2 - y1, 2)));
}

double getPerimeter(const std::vector<PointTuple> &points)
{

    double sum = 0;
    int x1, y1, x2, y2;

    auto firstPoint = points[0];

    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        auto nextPoint = firstPoint;

        if (i < points.size() - 1)
        {
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


float getCircularity(float area, float perimeter){
    return pow(perimeter, 2) / (4 * M_PI * area);
}

Stats getStats(std::vector<float> values){

    auto stats = Stats();

    // Mean
    auto meanRes = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    
    // SD
    std::vector<float> squares;
    squares.resize(values.size());

    std::transform(values.begin(), values.end(), squares.begin(), [&meanRes](float val){return pow(meanRes - val, 2);});
    auto sdRes = sqrt(std::accumulate(squares.begin(), squares.end(), 0.0) / values.size());

    // Range
    auto max = std::max_element(values.begin(), values.end());
    auto min = std::min_element(values.begin(), values.end());

    float rangeRes = 0;

    if(max != values.end() && min != values.end()){
        rangeRes = *max - *min;
    }
    
    stats._mean = meanRes;
    stats._sd = sdRes;
    stats._range = rangeRes;

    return stats;
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

    std::cout << " number of components : " << objects.size() << endl; // Right now size of "objects" is the number of connected components

    // types definition
    trace.beginBlock("Greedy");

    Board2D aBoardLine;    // DrawLine display
    Board2D aBoardPolygon; // Polygon display


    // Stats storage
    std::vector<float> areas;
    std::vector<float> perimeters;
    std::vector<float> circularities;

    for (auto &currentComponent : objects)
    {
        std::cout << std::endl
                  << std::endl
                  << "========= New Grain ==========" << std ::endl
                  << std::endl;

        // ==================================== Step 4 ======================================

        // A Freeman chain code is a string composed by the coordinates of the first pixel, and the list of elementary displacements.
        std::stringstream ss(stringstream::in | stringstream::out);

        // Get the boundary cruve
        auto curve = getBoundary(currentComponent);

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

        // Displaying contour
        aBoardLine << SetMode((*(theContour.begin())).className(), "Grid");

        for (unsigned int i = 0; i < theContour.size(); i++)
        {
            auto firstP = theContour.getPoint(i);
            auto secondP = theContour.getPoint((i + 1) % theContour.size());
            aBoardLine << firstP;
            aBoardLine.drawLine(firstP[0], firstP[1],
                                secondP[0], secondP[1]);
        }

        // Segmentation
        Decomposition4 theDecomposition(theContour.begin(), theContour.end(), DSS4());

        // Draw each segment
        string styleName = "";

        for (Decomposition4::SegmentComputerIterator
                 it = theDecomposition.begin(),
                 itEnd = theDecomposition.end();
             it != itEnd; ++it)
        {
            aBoardPolygon << SetMode("ArithmeticalDSS", "Points")
                          << it->primitive();
            aBoardPolygon << SetMode("ArithmeticalDSS", "BoundingBox")
                          << CustomStyle("ArithmeticalDSS/BoundingBox",
                                         new CustomPenColor(Color::Blue))
                          << it->primitive();
        }

        // ==================================== Step 5 ======================================

        std::cout << "AREA : " << std::endl
                  << std::endl;

        auto areaGridPoints = currentComponent.size();
        std::cout << "From grid points : " << areaGridPoints << std::endl;

        // Shoelace Computation
        std::vector<PointTuple> polygonPoints;

        for (auto &segment : theDecomposition)
        {
            auto x = (*(segment.begin()))[0];
            auto y = (*(segment.begin()))[1];
            polygonPoints.push_back(PointTuple(x, y));
        }

        auto areaPolygon = applyShoelace(polygonPoints);
        areas.push_back(areaPolygon);
        std::cout << "From Polygon shape : " << areaPolygon << std::endl;

        // ==================================== Step 6 ======================================

        std::cout << std::endl
                  << "Perimeter : " << std::endl
                  << std::endl;

        auto perimeterFromBoundary = curve.size();
        std::cout << "From Boundary : " << perimeterFromBoundary << std::endl;

        auto perimeterPolygon = getPerimeter(polygonPoints);
        perimeters.push_back(perimeterPolygon);

        std::cout << "From Polygon shape : " << perimeterPolygon << std::endl;

        // ==================================== Step 7 ======================================

        auto circularity = getCircularity(areaPolygon, perimeterPolygon);  // Use the multigrid convergent methods for area and perimeter
        circularities.push_back(circularity);

        std::cout << std::endl
                  << "Circularity : " << circularity << std::endl;
    }

    std::cout << std::endl
              << "End of the analysis !" << std::endl;

    aBoardLine.saveSVG("greedyOutLineVersion.svg", 600, 600, 10);
    aBoardPolygon.saveSVG("Polygonization.svg", 600, 600, 10);


    // ==================================== Step 8 ======================================


    std::cout << std::endl << "======= (Statistic presentation) =======" << std::endl << std::endl;

    std::cout << "< AREA > " << std::endl << getStats(areas) << std::endl;
    std::cout << "< PERIMETER > " << std::endl << getStats(perimeters) << std::endl;
    std::cout << "< CIRCULARITY > " << std::endl << getStats(circularities) << std::endl;


    return 0;
}
