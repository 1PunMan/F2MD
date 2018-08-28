/*
 * MLCheck.cc
 *
 *  Created on: Aug 20, 2018
 *      Author: mps
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>

#include <python2.7/Python.h>

#include <veins/modules/application/misbehaviorDetection/mdChecks/MLCheck.h>

bool shouldRun = true;

MLCheck::
MLCheck(const std::string& savedModelsPath, const std::string& mlResultsPath)
    : savedModelsPath_(savedModelsPath),
      mlResultsPath_(mlResultsPath)
{}



MLCheck::
~MLCheck() {}



int MLCheck::
callPlugin(char* modelName) {
//  CPyObject pName = PyUnicode_FromString(modelName.c_str());
//  PyObject* pluginModule = PyImport_Import(pName);
//  if (!pluginModule) {
//    PyErr_Print();
//    std::cout << "Error importing module" << std::endl;
//    return -1;
//  }
//
//  const char* pFuncName = "get_prediction"; // this function HAS to be present in the model
//  CPyObject predictionFunc = PyObject_GetAttrString(pluginModule, pFuncName);
//  Py_DECREF(pluginModule);
//  if (!predictionFunc) {
//    PyErr_Print();
//    std::cout << "Error retrieving 'get_prediction'" << std::endl;
//    return -1;
//  }
//
//  CPyObject pResult = PyObject_CallObject(predictionFunc, NULL);
//  if (!pResult) {
//    PyErr_Print();
//    std::cout << "Error invoking 'predictionFunc'" << std::endl;
//    return -1;
//  }

  //const char* mName = modelName.c_str();
  //char* fMode = (char*)"r";
//  FILE* fp = fopen(modelName, "r");
//  CPyObject fileObject = PyFile_FromFile(fp, modelName, (char*)"r", NULL);
  if (shouldRun) {
    PyRun_SimpleString("from sklearn.externals import joblib");
    PyRun_SimpleString("import numpy as np");
    shouldRun = false;
  }
  PyRun_SimpleString("import json");
  PyRun_SimpleString("import os");

  FILE* fp = fopen(modelName, "r");
  if (fp == NULL) {
    perror("Couldn't open file");
    return -1;
  }
  PyRun_SimpleFile(fp, modelName);
  fclose(fp);

//  CPyObject fileObject = PyFile_FromString(modelName, (char*)"r");
//  if (!fileObject) {
//    PyErr_Print();
//    std::cout << "Error importing ML model" << std::endl;
//    return -1;
//  }

//  PyRun_SimpleFile(PyFile_AsFile(fileObject), modelName);

  return 0;
}



void MLCheck::
printResult() {
  auto os = std::ostringstream{};
  os << mlResultsPath_ << "/pred_file.json";
  std::ifstream inFile(os.str());
  if (inFile.is_open()) {
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std:: endl;
    }
    inFile.close();
  }

  if(remove(os.str().c_str()) != 0)
    perror("Error deleting file");
  else
    puts("File successfully deleted");
}



int MLCheck::
getPrediction(char* modelName) {
  CPyInstance hInstance;
  CPyObject sysPath = PySys_GetObject((char*)"path");
  CPyObject mlPredictorPath = PyBytes_FromString(savedModelsPath_.c_str());
  PyList_Append(sysPath, mlPredictorPath);

  if (callPlugin(modelName) == -1)
    return -1;
  printResult(); // optional call

  return 0;
}
