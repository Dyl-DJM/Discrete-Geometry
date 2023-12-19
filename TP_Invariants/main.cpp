#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageSelector.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/io/viewers/Viewer3D.h>
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>

using namespace std;
using namespace DGtal;
using namespace Z3i;

int main(int argc, char **argv)
{
    typedef Object<DT26_6, Z3i::DigitalSet> DigitalObj3D;
    typedef ImageSelector<Z3i::Domain, int>::Type Image3D;

    // 3D scene storage
    std::vector<std::string> scenes3D = {"fertility", "dragon"};

    // I want my programm to iterate on all the scenes 
    for(const auto &scene : scenes3D){
        std::cout << "\033[1;32mbold ===" + scene + " analysis ===\033[0m" << std::endl;

        // read a 3D image
        Image3D image = VolReader<Image3D>::importVol("../3D_images/" + scene + "-128.vol");

        // make the foreground and background digital sets from the image
        Z3i::DigitalSet set_foreground(image.domain()); // Create a digital set of proper size
        // Z3i::DigitalSet set_background ( image.domain() );
        SetFromImage<Z3i::DigitalSet>::append<Image3D>(set_foreground, image, 0, 255); // populate a digital set from the input image
        // SetFromImage<Z3i::DigitalSet>::append<Image3D>(set_background, image, -1, 1);

        // =================== STEP 2 ===================
        DigitalObj3D digitalObj(dt26_6, set_foreground);
        std::vector<DigitalObj3D> objects;                                      // All connected components are going to be stored in it
        std::back_insert_iterator<std::vector<DigitalObj3D>> inserter(objects); // Iterator used to populated "objects".

        unsigned int nbc = digitalObj.writeComponents(inserter);

        std::cout << "Number of components : " << nbc << std::endl;

        // 3D viewer
        QApplication application(argc, argv);

        typedef Viewer3D<> MyViewer;
        MyViewer viewer;
        viewer.show();
        // viewer << shape;
        viewer << SetMode3D(image.domain().className(), "BoundingBox");
        viewer << set_foreground << image.domain() << MyViewer::updateDisplay;

        application.exec();
    }
}
