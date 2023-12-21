#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageSelector.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/io/viewers/Viewer3D.h>
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>

// Cellular grid
#include <DGtal/topology/CubicalComplex.h>

// Vector manipulation
#include <algorithm>
#include <numeric>
#include <tuple>

/* Namespaces */
using namespace std;
using namespace DGtal;
using namespace Z3i;

/* Types */
typedef Object<DT26_6, Z3i::DigitalSet> DigObj26_6; // STEP 2 : 3D digital object
typedef Object<DT6_26, Z3i::DigitalSet> DigObj6_26; // STEP 2 : 3D digital object

typedef map<Cell, CubicalCellData> Map; // STEP 3
typedef CubicalComplex<KSpace, Map> CC; // STEP 3

/* STEP 2 : Get the number of components, thanks to WriteComponents() */
template <typename Object>
unsigned int getNbComponents(const Object &digitalObj, std::vector<Object> &objects)
{
    std::back_insert_iterator<std::vector<Object>> inserter(objects); // Iterator used to populated "objects".
    unsigned int nbc = digitalObj.writeComponents(inserter);
    return nbc;
}

/* STEP 2 : Get the number of cavites and components */
template <typename ObjectType, typename Adjency>
std::tuple<int, int> getComponentsCavities(const std::string &&title,
                                           const Z3i::DigitalSet &set_foreground,
                                           const Z3i::DigitalSet &set_background,
                                           const Adjency &set)
{
    // Foreground
    ObjectType DOForeground(set, set_foreground);
    std::vector<ObjectType> objectsForeground; // All connected components are going to be stored in it
    auto nbcForeground = getNbComponents(DOForeground, objectsForeground);

    // Background
    ObjectType DOBackground(set, set_background);
    std::vector<ObjectType> objectsBackground; // All connected components are going to be stored in it
    auto nbcBackground = getNbComponents(DOBackground, objectsBackground);

    std::cout << title << std::endl;
    std::cout << "Number of components : " << nbcForeground << std::endl;
    std::cout << "Number of cavities : " << nbcBackground << std::endl;

    return std::tuple<int, int>{nbcForeground, nbcBackground};
}

/* STEP 3 : Creates a new digital object with the binding addjency and topology */
template <typename ObjectType, typename Adjency>
ObjectType createDigitalObj(const Z3i::DigitalSet &set,
                            const Adjency &adjency,
                            std::vector<ObjectType> &components)
{
    // Foreground
    ObjectType obj(adjency, set);
    std::back_insert_iterator<std::vector<ObjectType>> inserter(components); // Iterator used to populated "objects".
    obj.writeComponents(inserter);
    return obj;
}

/* STEP 3 : Computes Euler characteristic from number of cells */
int computeEuler(const std::vector<unsigned int> &vec)
{
    int acc = 0;
    int sign = 1;
    for (const auto elem : vec)
    {
        acc += (elem * sign);
        sign *= -1;
    }
    return acc;
}

/* STEP 4 : Computes the number of tunnels in the 3D images */
int getNbTunnels(int nbComponents, int nbCavities, int euler)
{
    return euler + nbCavities - nbComponents;
}

int main(int argc, char **argv)
{
    typedef ImageSelector<Z3i::Domain, int>::Type Image3D;

    // 3D scene storage
    std::vector<std::string> scenes3D = {"fertility", "dragon", "filigree", "torus"};

    // I want my programm to iterate on all the scenes
    for (const auto &scene : scenes3D)
    {
        std::cout << "\033[1;32m === " + scene + " analysis ===\033[0m" << std::endl;

        // 3D viewer
        // QApplication application(argc, argv);

        // typedef Viewer3D<> MyViewer;
        // MyViewer viewer;
        // viewer.show();

        // read a 3D image
        Image3D image = VolReader<Image3D>::importVol("../3D_images/" + scene + "-128.vol");

        // make the foreground and background digital sets from the image
        Z3i::DigitalSet set_foreground(image.domain()); // Create a digital set of proper size
        Z3i::DigitalSet set_background(image.domain());

        SetFromImage<Z3i::DigitalSet>::append<Image3D>(set_foreground, image, 0, 255); // populate a digital set from the input image
        SetFromImage<Z3i::DigitalSet>::append<Image3D>(set_background, image, -1, 1);

        // // viewer << shape;
        // viewer << SetMode3D(image.domain().className(), "BoundingBox");
        // viewer << set_foreground << image.domain() << MyViewer::updateDisplay;

        // =========================== STEP 2 ===================================
        auto nbCompCavs26 = getComponentsCavities<DigObj26_6, DT26_6>("(26, 6) Adjency : ", set_foreground, set_background, dt26_6);
        auto nbCompCavs6 = getComponentsCavities<DigObj6_26, DT6_26>("(6, 26) Adjency : ", set_foreground, set_background, dt6_26);

        // =========================== STEP 3 ===================================
        std::vector<DigObj26_6> objects;
        auto digObjForeground26 = createDigitalObj<DigObj26_6, DT26_6>(set_foreground, dt26_6, objects);

        KSpace kSpace; // Topological space

        kSpace.init(digObjForeground26.domain().lowerBound() - Point(1, 1, 1),
                    digObjForeground26.domain().upperBound() + Point(1, 1, 1), true);
        CC complex(kSpace);
        complex.construct<DigitalSet>(set_foreground);

        auto nb0 = complex.nbCells(0);
        auto nb1 = complex.nbCells(1);
        auto nb2 = complex.nbCells(2);
        auto nb3 = complex.nbCells(3);

        const std::vector<unsigned int> elements = {nb0, nb1, nb2, nb3};

        std::cout
            << std::endl
            << "0-cells : " << complex.nbCells(0) << std::endl;
        std::cout << "1-cells : " << complex.nbCells(1) << std::endl;
        std::cout << "2-cells : " << complex.nbCells(2) << std::endl;
        std::cout << "3-cells : " << complex.nbCells(3) << std::endl;

        std::cout << std::endl
                  << "Euler (with complex.euler()) : " << complex.euler() << std::endl;

        std::cout << "Euler (with formula) : " << computeEuler(elements) << std::endl;

        // =========================== STEP 4 ===================================

        // Initialize the
        std::vector<DigObj6_26> objectsBis;
        auto digObjForeground6 = createDigitalObj<DigObj6_26, DT6_26>(set_foreground, dt6_26, objectsBis);

        KSpace kSpace6; // Topological space

        kSpace6.init(digObjForeground6.domain().lowerBound() - Point(1, 1, 1),
                     digObjForeground6.domain().upperBound() + Point(1, 1, 1), true);
        CC complex6(kSpace6);
        complex6.construct<DigitalSet>(set_foreground);

        auto tunnels26_6 = getNbTunnels(std::get<0>(nbCompCavs26), std::get<1>(nbCompCavs26), complex.euler());
        auto tunnels6_26 = getNbTunnels(std::get<0>(nbCompCavs6), std::get<1>(nbCompCavs6), complex6.euler());

        std::cout << std::endl
                  << "Nb tunnels (26 Adjency): " << tunnels26_6 << std::endl;

        std::cout << "Nb tunnels (6 Adjency): " << tunnels6_26 << std::endl;

        // application.exec();

        std::cout << std::endl;
    }
}
