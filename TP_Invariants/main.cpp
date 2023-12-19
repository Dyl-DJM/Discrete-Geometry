#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageSelector.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/io/viewers/Viewer3D.h>
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>

/* Namespaces */
using namespace std;
using namespace DGtal;
using namespace Z3i;

/* Types */
typedef Object<DT26_6, Z3i::DigitalSet> DigObj26_6; // STEP 2 : 3D digital object
typedef Object<DT6_26, Z3i::DigitalSet> DigObj6_26; // STEP 2 : 3D digital object

/* STEP 2 : Get the number of components, thanks to WriteComponents() */
template <typename Object>
unsigned int getNbComponents(const Object &digitalObj, std::vector<Object> &objects)
{
    std::back_insert_iterator<std::vector<Object>> inserter(objects); // Iterator used to populated "objects".
    unsigned int nbc = digitalObj.writeComponents(inserter);
    return nbc;
}

template <typename ObjectType, typename Adjency>
void ComponentsCavities(const std::string &&title,
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
        QApplication application(argc, argv);

        typedef Viewer3D<> MyViewer;
        MyViewer viewer;
        viewer.show();

        // read a 3D image
        Image3D image = VolReader<Image3D>::importVol("../3D_images/" + scene + "-128.vol");

        // make the foreground and background digital sets from the image
        Z3i::DigitalSet set_foreground(image.domain()); // Create a digital set of proper size
        Z3i::DigitalSet set_background(image.domain());

        SetFromImage<Z3i::DigitalSet>::append<Image3D>(set_foreground, image, 0, 255); // populate a digital set from the input image
        SetFromImage<Z3i::DigitalSet>::append<Image3D>(set_background, image, -1, 1);

        // viewer << shape;
        viewer << SetMode3D(image.domain().className(), "BoundingBox");
        viewer << set_foreground << image.domain() << MyViewer::updateDisplay;

        // =================== STEP 2 ===================
        ComponentsCavities<DigObj26_6, DT26_6>("(26, 6) Adjency : ", set_foreground, set_background, dt26_6);
        ComponentsCavities<DigObj6_26, DT6_26>("(6, 26) Adjency : ", set_foreground, set_background, dt6_26);

        application.exec();

        std::cout << std::endl;
    }
}
