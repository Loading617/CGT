#ifndef _CGT_CGT_h
#define _CGT_CGT_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct ImageItem {
    Image img;
    Point pos;
    Size size;
};

class CGTApp : public TopWindow {
public:
    CGTApp();

    virtual void Paint(Draw& w) override;
    virtual void LeftDown(Point p, dword keyflags) override;
    virtual void MouseMove(Point p, dword keyflags) override;
    virtual void LeftUp(Point p, dword keyflags) override;

private:
    Vector<ImageItem> images;
    int selectedImage = -1;
    Point dragOffset;

    void LoadImages();
    void ExportCollage();
};

#endif