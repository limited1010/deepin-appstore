
#ifndef SHELL_WEBKIT_WEBVIEW_H
#define SHELL_WEBKIT_WEBVIEW_H

#include <QWebView>
#include "WebPage.h"

class WebView : public QWebView {
    Q_OBJECT

public:
    explicit WebView(QWidget* parent = nullptr);
    ~WebView();

private:
    void resizeEvent(QResizeEvent* event);
    WebPage* customPage;
    void polish();
    const unsigned borderRadius = 3;

};


#endif //SHELL_WEBKIT_WEBVIEW_H
