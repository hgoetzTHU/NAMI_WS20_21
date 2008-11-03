/*=========================================================================

 Program:   openCherry Platform
 Language:  C++
 Date:      $Date$
 Version:   $Revision$

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include "cherryQCTabBar.h"

#include <QApplication>
#include <QMouseEvent>

#include <algorithm>

namespace cherry
{

QCTabBar::QCTabBar(QWidget* parent) :
  QTabBar(parent)
{

}

QCTabBar::~QCTabBar()
{
  for (std::deque<AbstractTabItem*>::iterator iter = tabItemList.begin();
       iter != tabItemList.end(); ++iter)
  {
    delete *iter;
  }
}

void QCTabBar::tabRemoved(int index)
{
  std::deque<AbstractTabItem*>::iterator iter = tabItemList.begin();
  std::advance(iter, index);
  tabItemList.erase(iter);
  delete *iter;
}

void QCTabBar::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
     dragStartPosition = event->pos();

  QTabBar::mousePressEvent(event);
}

void QCTabBar::mouseMoveEvent(QMouseEvent* event)
{
  if (!(event->buttons() & Qt::LeftButton))
  {
    QTabBar::mouseMoveEvent(event);
    return;
  }

   if ((event->pos() - dragStartPosition).manhattanLength()
        < QApplication::startDragDistance())
   {
     QTabBar::mouseMoveEvent(event);
     return;
   }

   emit dragStarted(event->globalPos());
}

AbstractTabItem* QCTabBar::getTab(int index) const
{
  return tabItemList[index];
}

std::vector<AbstractTabItem*> QCTabBar::getTabs() const
{
  return std::vector<AbstractTabItem*>(tabItemList.begin(), tabItemList.end());
}

void QCTabBar::insertTab(int index, AbstractTabItem* item)
{
  std::deque<AbstractTabItem*>::iterator iter = tabItemList.begin();
  std::advance(iter, index);
  tabItemList.insert(iter, item);
  QTabBar::insertTab(index, QString());
}

void QCTabBar::setCurrentTab(AbstractTabItem* item)
{
  std::deque<AbstractTabItem*>::iterator iter = std::find(tabItemList.begin(), tabItemList.end(), item);
  int index = iter - tabItemList.begin();
  this->setCurrentIndex(index);
}

AbstractTabItem* QCTabBar::getCurrentTab()
{
  int index = this->currentIndex();
  return tabItemList[index];
}

}
