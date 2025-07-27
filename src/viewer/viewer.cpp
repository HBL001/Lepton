#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QWidget>
#include <vector>

class ViewerWidget : public QWidget {
public:
    ViewerWidget(QWidget* parent = nullptr) : QWidget(parent) {
        // Simulate 80x60 Lepton frame, grayscale
        fakeFrame.resize(80 * 60, 0);
        for (int i = 0; i < 80 * 60; ++i)
            fakeFrame[i] = static_cast<uint8_t>((i % 255));

        // Convert to QImage (RGB888 for simplicity)
        image = QImage(80, 60, QImage::Format_RGB888);
        for (int y = 0; y < 60; ++y) {
            for (int x = 0; x < 80; ++x) {
                int v = fakeFrame[y * 80 + x];
                image.setPixel(x, y, qRgb(v, v, v));
            }
        }

        setFixedSize(image.size() * 5); // Scale for visibility
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.scale(5, 5);  // 5x zoom for HDMI visibility
        painter.drawImage(0, 0, image);
    }

private:
    QImage image;
    std::vector<uint8_t> fakeFrame;
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    ViewerWidget viewer;
    viewer.setWindowTitle("Lepton HDMI Viewer");
    viewer.show();
    return app.exec();
}
