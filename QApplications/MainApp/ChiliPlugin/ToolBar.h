#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <qwidget.h>
#include <qlayout.h>
#include <qclightbox.h>
#include <chili/plugin.h>

class QWidget;
class QGridLayout;
class QButtonGroup;

class ToolBar: public QWidget
{
    Q_OBJECT

public:
    ToolBar(QWidget* parent,QcPlugin* qcplugin);
    ~ToolBar();
    void SetWidget(QWidget* ap);
    QButtonGroup* GetToolBar();
    void ConnectButton(int number);
    bool KeepDataTreeNodes();
    
public slots:
    // restarts the application - the bool parameter can force a reinit. 
    // usaly there is a check if something was loaded - to save time
    void Reinitialize(bool );
    // Lightbox action 1-4 -> light box will be added or replaced in the mitk data tree
    void ButtonToggled(int );
    // to change the state to use multiple images in the data tree or only a single one
    void KeepDataTreeNodesCBtoggled(bool );

signals:
    void LightboxSelected(QcLightbox*);
    void ChangeWidget(bool=false);
        
private:
    QWidget* widget;
    QWidget* task;
    QcPlugin* plugin;
    QGridLayout* layout;
    QButtonGroup* toolbar;    
    int idLightbox;
    void SelectLightbox(int id);
    void ResetLightboxButtons(int select=0);
    bool m_KeepDataTreeNodes;
    bool m_KeepDataTreeReinitNeeded;
};

#endif
