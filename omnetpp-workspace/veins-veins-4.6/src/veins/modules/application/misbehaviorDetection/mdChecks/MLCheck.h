/*******************************************************************************
 * @author  Raashid Ansari
 * @email   m.raashid.ansari@gmail.com
 * @date    08/20/2018
 * @version 1.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * Copyright (c) 2018 OnBoard Security, Inc.
 * All rights reserved.
 *******************************************************************************/


#ifndef SRC_VEINS_MODULES_APPLICATION_MISBEHAVIORDETECTION_MDCHECKS_MLCHECK_H_
#define SRC_VEINS_MODULES_APPLICATION_MISBEHAVIORDETECTION_MDCHECKS_MLCHECK_H_

#include <string>

#include <veins/modules/application/misbehaviorDetection/supportClasses/pyhelper.h>

class MLCheck {
public:
    explicit MLCheck(const std::string& savedModelsPath, const std::string& mlResultsPath);
    virtual ~MLCheck();
    int callPlugin(char* modelName);
    void printResult();
    int getPrediction(char* modelName);
private:
    std::string savedModelsPath_;
    std::string mlResultsPath_;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_MISBEHAVIORDETECTION_MDCHECKS_MLCHECK_H_ */
