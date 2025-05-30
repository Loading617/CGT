#include "CGT.h"

CGTApp::CGTApp() {
    Title("CGT");
    Sizeable().Zoomable();
    SetRect(0, 0, 800, 600);

    AddFrame(MenuBarFrame([=](Bar& bar) {
        bar.Add("File", [=](Bar& menu) {
            menu.Add("Load Images", [=] { LoadImages(); });
            menu.Add("Export Collage", [=] { ExportCollage(); });
            menu.Separator();
            menu.Add("Exit", [=] { Close(); });
        });
    }));
}

void CGTApp::LoadImages() {
    FileSel fs;
    fs.Type("Images", "*.png;*.jpg;*.jpeg;*.bmp");
    fs.Multi();
    if (fs.ExecuteOpen()) {
        for (const auto& file : fs) {
            Image img = StreamRaster::LoadAny(file);
            if (!img.IsEmpty()) {
                images.Add({img, Point(100, 100), img.GetSize()});
            }
        }
        Refresh();
    }
}

void CGTApp::ExportCollage() {
    FileSel fs;
    fs.Type("PNG Image", "*.png");
    if (fs.ExecuteSaveAs()) {
        Size sz = GetSize();
        ImageBuffer out(sz);
        PaintingPainter p(out);
        p.Clear(White());

        for (const auto& item : images) {
            p.DrawImage(item.pos.x, item.pos.y, item.img);
        }

        PNGEncoder().SaveFile(fs.Get(), out);
    }
}

void CGTApp::Paint(Draw& w) {
    w.DrawRect(GetSize(), White());

    for (const auto& item : images) {
        w.DrawImage(item.pos.x, item.pos.y, item.img);
    }
}

void CGTApp::LeftDown(Point p, dword) {
    for (int i = images.GetCount() - 1; i >= 0; --i) {
        Rect r(images[i].pos, images[i].size);
        if (r.Contains(p)) {
            selectedImage = i;
            dragOffset = p - images[i].pos;
            return;
        }
    }
    selectedImage = -1;
}

void CGTApp::MouseMove(Point p, dword keyflags) {
    if ((keyflags & K_MOUSELEFT) && selectedImage >= 0) {
        images[selectedImage].pos = p - dragOffset;
        Refresh();
    }
}

void CGTApp::LeftUp(Point, dword) {
    selectedImage = -1;
}