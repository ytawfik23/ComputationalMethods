// SoQt includes
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoTranslation.h>

#include <QApplication>
#include <QWidget>
int main(int argc, char **argv)
{

#ifndef __APPLE__
  setenv("QT_QPA_PLATFORM","xcb",0);
#endif

  
  // Initialize the Qt system:
  QApplication app(argc, argv);
  
  // Make a main window:
  QWidget mainwin;
  mainwin.resize(400,400);

  // Initialize SoQt 
  SoQt::init(&mainwin);
  
  // The root of a scene graph
  SoSeparator *root = new SoSeparator;
  root->ref();

  SoBaseColor *color = new SoBaseColor;
  color->rgb = SbColor(1, 1, 0); 
  SoTranslation * trans = new SoTranslation;
  trans->translation.setValue(2,0,0);
  SoCube *cube = new SoCube;

  root->addChild(color);
  root->addChild(cube);
  root->addChild(trans);
  root->addChild(color);
  root->addChild(cube);

  // Initialize an examiner viewer:
  SoQtExaminerViewer * eviewer = new SoQtExaminerViewer(&mainwin);
  eviewer->setDoubleBuffer(false); // Needed for Wayland window manager.
  
  eviewer->setSceneGraph(root);
  eviewer->show();
  
  // Pop up the main window.
  SoQt::show(&mainwin);

  // Loop until exit.
  SoQt::mainLoop();

  // Clean up resources.
  delete eviewer;
  root->unref();
}
