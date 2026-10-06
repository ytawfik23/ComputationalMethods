#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"

#include "QatGenericFunctions/InterpolatingFunction.h"
#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotFunction1D.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

std::vector<std::vector<double>> extendData(const std::vector<std::vector<double>>& data) {
    std::vector<std::vector<double>> extendedData = data;
    double finalWavelength = data[data.size() - 1][0];
    double finalValue = data[data.size() - 1][1];
    double finalSlope = (finalValue - data[data.size() - 2][1]) / (finalWavelength - data[data.size() - 2][0]);
    
    double kappa = -finalSlope / finalValue;
    double A = finalValue * std::exp(kappa * finalWavelength);

    for(double x = finalWavelength + 10; x <= 700.; x += 10.) {
        extendedData.push_back({x, A * std::exp(-kappa * x)});
    }
    return extendedData;
}

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }
 
  std::string filename_red = "../src/cones_red.csv";
  std::string filename_green = "../src/cones_green.csv";
  std::string filename_blue = "../src/cones_blue.csv";

  std::ifstream fileRed(filename_red);
  if (!fileRed.is_open()){
    std::cerr << "Error: could not open file " << filename_red << std::endl;
  }
  std::ifstream fileGreen(filename_green);
  if (!fileGreen.is_open()){
    std::cerr << "Error: could not open file " << filename_green << std::endl;
  }
  std::ifstream fileBlue(filename_blue);
  if (!fileBlue.is_open()){
    std::cerr << "Error: could not open file " << filename_blue << std::endl;
  }

  std::vector<std::vector<double>> redData, greenData, blueData;

  std::string line;
  while(std::getline(fileRed, line)){
    std::vector<double> row;
    std::stringstream ss(line);    
    std::string cell;
    while (std::getline(ss, cell, ',')){
      row.push_back(stod(cell));
    }
    redData.push_back(row);
  }
  while(std::getline(fileGreen, line)){
    std::vector<double> row;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')){
      row.push_back(stod(cell));
    }
    greenData.push_back(row);
  }
  while(std::getline(fileBlue, line)){
    std::vector<double> row;
    std::stringstream ss(line);    
    std::string cell;
    while (std::getline(ss, cell, ',')){
      row.push_back(stod(cell));
    }
    blueData.push_back(row);
  }

  PlotProfile plotpointsRed, plotpointsGreen, plotpointsBlue;
  for(int i=0; i<redData.size(); i++) {
    plotpointsRed.addPoint(redData[i][0], redData[i][1]);
  }
  for(int i=0; i<greenData.size(); i++) {
    plotpointsGreen.addPoint(greenData[i][0], greenData[i][1]);
  }
  for(int i=0; i<blueData.size(); i++) {
    plotpointsBlue.addPoint(blueData[i][0], blueData[i][1]);
  }

  Genfun::InterpolatingFunction 
    interpolationRed(Genfun::CUBIC_SPLINE), 
    interpolationGreen(Genfun::CUBIC_SPLINE),
    interpolationBlue(Genfun::CUBIC_SPLINE);
  std::vector<std::vector<double>> extendedRedData = extendData(redData);  
  std::vector<std::vector<double>> extendedGreenData = extendData(greenData);
  std::vector<std::vector<double>> extendedBlueData = extendData(blueData);
  
  for(int i=0; i<extendedRedData.size(); i++) {
    interpolationRed.addPoint(extendedRedData[i][0], extendedRedData[i][1]);
  }
  for(int i=0; i<extendedGreenData.size(); i++) {
    interpolationGreen.addPoint(extendedGreenData[i][0], extendedGreenData[i][1]);
  }
  for(int i=0; i<extendedBlueData.size(); i++) {
    interpolationBlue.addPoint(extendedBlueData[i][0], extendedBlueData[i][1]);
  }
  
  
  fileRed.close();
  fileGreen.close();
  fileBlue.close();

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  PRectF rect;
  rect.setXmin(400.0);
  rect.setXmax(700.0);
  rect.setYmin(0.0);
  rect.setYmax(110.0);
  


  PlotView view(rect);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "Cone Absorption"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Wavelength (nm)"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Absorption"
	       << PlotStream::EndP();
  
  PlotFunction1D interpolationRedPlot{interpolationRed}, 
    interpolationGreenPlot{interpolationGreen},
    interpolationBluePlot{interpolationBlue};

  PlotProfile::Properties interpolationRedProperties, 
    interpolationGreenProperties,
    interpolationBlueProperties;

  interpolationRedProperties.pen.setColor("red");
  interpolationRedProperties.pen.setWidth(4);
  interpolationGreenProperties.pen.setColor("green");
  interpolationGreenProperties.pen.setWidth(4);
  interpolationBlueProperties.pen.setColor("blue");
  interpolationBlueProperties.pen.setWidth(4);
  interpolationRedProperties.pen.setWidth(4);

  plotpointsRed.setProperties(interpolationRedProperties);
  plotpointsGreen.setProperties(interpolationGreenProperties);
  plotpointsBlue.setProperties(interpolationBlueProperties);

  view.add(&interpolationRedPlot);
  view.add(&plotpointsRed);
  view.add(&interpolationGreenPlot);
  view.add(&plotpointsGreen);
  view.add(&interpolationBluePlot);
  view.add(&plotpointsBlue);


    window.show();
  app.exec();
  return 1;
}

