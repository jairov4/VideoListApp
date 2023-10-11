#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>
#include <QFileInfoList>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QtWin>
#include <Windows.h>
#include <Shobjidl.h>
#include <Shlobj.h>
#include <initguid.h>
#include <thumbcache.h>


class VideoListApp : public QWidget {
    Q_OBJECT

public:
    VideoListApp(QWidget *parent = nullptr) : QWidget(parent) {
        CoInitialize(nullptr);
        CoCreateInstance(CLSID_LocalThumbnailCache, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&thumbnailCache));
        initUI();
    }

    virtual ~VideoListApp() {
        thumbnailCache->Release();
        CoUninitialize();
    }

private slots:
    void changeDirectory() {
        QString newDirectory = directoryEdit->text();
        if (QDir(newDirectory).exists()) {
            qInfo() << "Changing directory";
            QDir::setCurrent(newDirectory);
            populateList();
        }
    }

    void searchVideos(const QString &searchText) {
        if (searchText.isEmpty()) {
            populateList();
            return;
        }

        listWidget->clear();

        QStringList videoExtensions = {"*.mp4", "*.mpg", "*.avi", "*.mkv"};
        QDirIterator it(QDir::currentPath(), videoExtensions, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QString videoPath = it.next();
            QString videoName = QFileInfo(videoPath).fileName();

            if (videoName.contains(searchText, Qt::CaseInsensitive)) {
                populateListItem(videoPath);
            }
        }
    }

    void playVideo(QListWidgetItem *item) {
        QString videoPath = item->whatsThis();
        QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));
    }

private:
    void initUI() {
        setWindowTitle("Video List");
        setGeometry(100, 100, 400, 500);

        // Create a vertical layout for the widgets
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Create a line edit for entering the directory path
        directoryEdit = new QLineEdit(this);
        layout->addWidget(directoryEdit);

        // Create a button to confirm the directory change
        QPushButton *directoryButton = new QPushButton("Change Directory", this);
        layout->addWidget(directoryButton);

        // Create a line edit for searching videos
        QLineEdit *searchEdit = new QLineEdit(this);
        layout->addWidget(searchEdit);

        // Create a list widget to display the videos
        listWidget = new QListWidget(this);
        layout->addWidget(listWidget);

        setLayout(layout);

        // Set the initial directory to the current directory
        directoryEdit->setText(QDir::currentPath());

        // Populate the list with videos from the current directory
        populateList();

        // Connect the signals and slots
        connect(directoryButton, &QPushButton::clicked, this, &VideoListApp::changeDirectory);
        connect(searchEdit, &QLineEdit::textChanged, this, &VideoListApp::searchVideos);
        connect(listWidget, &QListWidget::itemDoubleClicked, this, &VideoListApp::playVideo);
    }

    void populateList() {
        listWidget->clear();

        QStringList videoExtensions = {"*.mp4", "*.mpg", "*.avi", "*.mkv"};
        QDirIterator it(QDir::currentPath(), videoExtensions, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QString videoPath = it.next();
            populateListItem(videoPath);
        }
    }

    void populateListItem(QString videoPath) {
        QFileInfo videoInfo(videoPath);

        auto item = new QListWidgetItem();
        item->setWhatsThis(videoInfo.filePath());
        item->setSizeHint(QSize(0, 240));
        auto pixmap = extractThumbnail(videoInfo.absoluteFilePath());
        auto wight = new QWidget();
        auto layout = new QHBoxLayout();
        auto label0 = new QLabel();
        label0->setPixmap(pixmap);
        label0->setFixedSize(320, 240);
        auto label1 = new QLabel(videoInfo.fileName());
        layout->addWidget(label0);
        layout->addWidget(label1);
        wight->setLayout(layout);

        listWidget->addItem(item);
        listWidget->setItemWidget(item, wight);
    }

    QPixmap extractThumbnail(QString videoPath) {
        IShellItem* shellItem;
        videoPath.replace("/", "\\");
        HRESULT hr = SHCreateItemFromParsingName(reinterpret_cast<LPCWSTR>(videoPath.utf16()), nullptr, IID_PPV_ARGS(&shellItem));
        if (FAILED(hr)) {
            //qWarning() << "Failed to create shell item for the video file." << videoPath;
            return QPixmap();
        }

        ISharedBitmap* bitmap = nullptr;

        int cXY = 200;
        WTS_FLAGS flags = WTS_EXTRACT;
        hr = thumbnailCache->GetThumbnail(shellItem, cXY, WTS_EXTRACT, &bitmap, nullptr, nullptr);
        if (FAILED(hr) || bitmap == nullptr) {
            //qWarning() << "Failed to extract thumbnail for the video file." << videoPath;
            return QPixmap();
        }

        HBITMAP hBitmap;
        hr = bitmap->GetSharedBitmap(&hBitmap);
        if (FAILED(hr)) {
            //qWarning() << "Failed to extract thumbnail for the video file." << videoPath;
            return QPixmap();
        }

        QPixmap pixmap = QtWin::fromHBITMAP(hBitmap);

        DeleteObject(hBitmap);
        bitmap->Release();
        shellItem->Release();
        return pixmap;
    }

    QLineEdit *directoryEdit;
    QListWidget *listWidget;
    IThumbnailCache* thumbnailCache;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    VideoListApp window;
    window.show();

    return app.exec();
}

#include "main.moc"
