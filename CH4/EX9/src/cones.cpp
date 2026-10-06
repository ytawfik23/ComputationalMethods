#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"

#include "QatGenericFunctions/InterpolatingFunction.h"
#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotFunction1D.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>

#include <eigen3/Eigen/Dense>
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
  
  fileRed.close();
  fileGreen.close();
  fileBlue.close();

  std::vector<std::vector<double>> extendedRedData = extendData(redData);
  std::vector<std::vector<double>> extendedGreenData = extendData(greenData);
  std::vector<std::vector<double>> extendedBlueData = extendData(blueData);

  Genfun::InterpolatingFunction interpolationRed(Genfun::CUBIC_SPLINE), interpolationGreen(Genfun::CUBIC_SPLINE), interpolationBlue(Genfun::CUBIC_SPLINE);

  for(int i=0; i<extendedRedData.size(); i++) {
    interpolationRed.addPoint(extendedRedData[i][0], extendedRedData[i][1]);
  }
  for(int i=0; i<extendedGreenData.size(); i++) {
    interpolationGreen.addPoint(extendedGreenData[i][0], extendedGreenData[i][1]);
  }
  for(int i=0; i<extendedBlueData.size(); i++) {
    interpolationBlue.addPoint(extendedBlueData[i][0], extendedBlueData[i][1]);
  }

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  int Pixedwidth = 1000;
  int Pixedheight = 300;
  QImage image(Pixedwidth, Pixedheight, QImage::Format_RGB32);
  QRgb rgbColor;
  double wavelength{0};
  
  for(int i=0; i<Pixedwidth;i++){
    wavelength = 400.0 + (i * 300.0 / Pixedwidth);

    double alpha = 255.0/(interpolationRed(wavelength) + interpolationGreen(wavelength) + interpolationBlue(wavelength));
    double r = alpha * interpolationRed(wavelength);
    double g = alpha * interpolationGreen(wavelength);
    double b = alpha * interpolationBlue(wavelength);
    
    for(int j=0; j<Pixedheight; j++){
      image.setPixel(i, j, qRgb(r, g, b));
    }
  }
  image.save("cones.png");

  
  //  window.show();
  //  app.exec();
  return 1;
}

