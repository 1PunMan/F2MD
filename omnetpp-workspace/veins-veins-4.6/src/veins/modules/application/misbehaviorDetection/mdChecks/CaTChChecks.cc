/*******************************************************************************
 * @author  Joseph Kamel 
 * @email   josephekamel@gmail.com
 * @date    11/04/2018
 * @version 1.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <veins/modules/application/misbehaviorDetection/mdChecks/CaTChChecks.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace boost;

CaTChChecks::CaTChChecks(unsigned long myPseudonym, Coord myPosition,
        Coord myPositionConfidence, Coord myHeading, Coord myHeadingConfidence,
        Coord mySize, LinkControl* LinkC) {
    this->myPseudonym = myPseudonym;
    this->myPosition = myPosition;
    this->myPositionConfidence = myPositionConfidence;
    this->myHeading = myHeading;
    this->myHeadingConfidence = myHeadingConfidence;
    this->mySize = mySize;
    this->LinkC = LinkC;
}

double CaTChChecks::RangePlausibilityCheck(Coord receiverPosition,
        Coord receiverPositionConfidence, Coord senderPosition,
        Coord senderPositionConfidence) {

    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);
    double senderR = senderPositionConfidence.x;
    double receiverR = receiverPositionConfidence.x;
    double factor = mdmLib.CircleCircleFactor(distance, senderR, receiverR,
    MAX_PLAUSIBLE_RANGE);

    return factor;
}

double CaTChChecks::PositionConsistancyCheck(Coord curPosition,
        Coord curPositionConfidence, Coord oldPosition,
        Coord oldPositionConfidence, double time) {
    double distance = mdmLib.calculateDistance(curPosition, oldPosition);
    double curR = curPositionConfidence.x;
    double oldR = oldPositionConfidence.x;

    double factor = mdmLib.CircleCircleFactor(distance, curR, oldR,
    MAX_CONSISTANT_DISTANCE * time);

    return factor;
}

double CaTChChecks::SpeedConsistancyCheck(double curSpeed,
        double curSpeedConfidence, double oldspeed, double oldSpeedConfidence,
        double time) {

    double speedDelta = curSpeed - oldspeed;

    double attFact = mdmLib.gaussianSum(1, 1 / 3);
    if (time >= 1) {
        attFact = time;
    }

    double factor = 1;
    if (speedDelta > 0) {
        factor = mdmLib.SegmentSegmentFactor(speedDelta, curSpeedConfidence,
                oldSpeedConfidence,
                MAX_PLAUSIBLE_ACCEL * attFact);
    } else {
        factor = mdmLib.SegmentSegmentFactor(speedDelta, curSpeedConfidence,
                oldSpeedConfidence,
                MAX_PLAUSIBLE_DECEL * attFact);
    }
    return factor;
}

double CaTChChecks::SpeedPlausibilityCheck(double speed,
        double speedConfidence) {
    if ((fabs(speed) + fabs(speedConfidence) / 2) < MAX_PLAUSIBLE_SPEED) {
        return 1;
    } else if ((fabs(speed) - fabs(speedConfidence) / 2) > MAX_PLAUSIBLE_SPEED) {
        return 0;
    } else {
        double factor = (fabs(speedConfidence) / 2
                + (MAX_PLAUSIBLE_SPEED - fabs(speed))) / fabs(speedConfidence);

        return factor;
    }
}

double CaTChChecks::PositionSpeedConsistancyCheckOld(Coord curPosition,
        Coord curPositionConfidence, Coord oldPosition,
        Coord oldPositionConfidence, double curSpeed, double curSpeedConfidence,
        double oldspeed, double oldSpeedConfidence, double time) {

    MDMLib mdmLib;

    if (time < MAX_TIME_DELTA) {

        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        double theoreticalSpeed = distance / time;
        double maxspeed = std::max(curSpeed, oldspeed);
        double minspeed = std::min(curSpeed, oldspeed);

        double curR = curPositionConfidence.x / time + curSpeedConfidence;
        double oldR = oldPositionConfidence.x / time + oldSpeedConfidence;

        double minfactor;

        minfactor = mdmLib.OneSidedCircleSegmentFactor(theoreticalSpeed, curR,
                oldR, maxspeed - MIN_PSS);

//        std::cout << " theoreticalSpeed:" << theoreticalSpeed << " curR:" << curR
//                << " oldR:" << oldR << " maxspeed - MIN_PSS:" << maxspeed - MIN_PSS
//                << '\n';

        double maxfactor;

        if (minspeed - MAX_PSS < 0) {
            maxfactor = 1;
        } else {
            maxfactor = mdmLib.OneSidedCircleSegmentFactor(theoreticalSpeed,
                    curR, oldR, minspeed - MAX_PSS);
            maxfactor = 1 - maxfactor;
        }

//        std::cout << " minfactor:" << minfactor << " maxfactor:" << maxfactor
//                << " theoreticalSpeed:" << theoreticalSpeed << " maxspeed:" << maxspeed
//                << " minspeed:" << minspeed << '\n';

        double factor = 1;

        if (minfactor < maxfactor) {
            factor = minfactor;
        } else {
            factor = maxfactor;
        }

//        factor = (factor - 0.5) * 2;
//        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
//        if (factor > 0.75) {
//            factor = 1;
//        }
//        if (factor <0.001) {
//            factor = 0;
//        }
        //  std::cout << " Old Min:" << minfactor << " Max:" << maxfactor << '\n';
        return factor;

    } else {
        return 1;
    }
}

double CaTChChecks::PositionSpeedConsistancyCheck(Coord curPosition,
        Coord curPositionConfidence, Coord oldPosition,
        Coord oldPositionConfidence, double curSpeed, double curSpeedConfidence,
        double oldspeed, double oldSpeedConfidence, double time) {

    MDMLib mdmLib;

    double attFact = mdmLib.gaussianSum(1, 1 / 3);
    if (time >= 1) {
        attFact = 1;
    }

    if (time < MAX_TIME_DELTA) {

        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        double theoreticalSpeed = distance / time;
        double maxspeed = std::max(curSpeed, oldspeed);
        double minspeed = std::min(curSpeed, oldspeed);

        double curR = curPositionConfidence.x / time + curSpeedConfidence;
        double oldR = oldPositionConfidence.x / time + oldSpeedConfidence;

        double maxfactor = mdmLib.OneSidedCircleSegmentFactor(
                maxspeed - theoreticalSpeed, curR, oldR, MAX_PSS);

        double minfactor = mdmLib.OneSidedCircleSegmentFactor(
                theoreticalSpeed - minspeed, curR, oldR, - MIN_PSS);

        double factor = 1;

        if (minfactor < maxfactor) {
            factor = minfactor;
        } else {
            factor = maxfactor;
        }

//        factor = (factor - 0.5) * 2;
//        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
//        if (factor > 0.75) {
//            factor = 1;
//        }
//
//        if (factor <0.001) {
//            factor = 0;
//        }

        double factorOld = PositionSpeedConsistancyCheckOld(curPosition,
                curPositionConfidence, oldPosition, oldPositionConfidence,
                curSpeed, curSpeedConfidence, oldspeed, oldSpeedConfidence,
                time);

        if (factor < 0) {
            std::cout << " Min:" << minfactor << " Max:" << maxfactor << '\n';
            std::cout << " time:" << time << '\n';
            std::cout << " MaxSpeed:" << maxspeed << '\n';
            std::cout << " minspeed:" << minspeed << '\n';
            std::cout << " theoreticalSpeed:" << theoreticalSpeed << '\n';

            if (factorOld != factor) {
                std::cout << " factorOld:" << factorOld << " factor:" << factor
                        << '\n';
                std::cout << "============================================"
                        << '\n';
            }
        }

        return factor;

    } else {
        return 1;
    }
}

double CaTChChecks::IntersectionCheck(Coord nodePosition1,
        Coord nodePositionConfidence1, Coord nodePosition2,
        Coord nodePositionConfidence2, Coord nodeHeading1, Coord nodeHeading2,
        Coord nodeSize1, Coord nodeSize2) {

//    double distance = mdmLib.calculateDistance(nodePosition1, nodePosition2);
//    double intFactor = mdmLib.CircleIntersectionFactor(
//            nodePositionConfidence1.x, nodePositionConfidence2.x, distance,
//            MIN_INT_DIST);
//    intFactor = 1 - intFactor;
//    return intFactor;

    double heading1 = mdmLib.calculateHeadingAngle(nodeHeading1);
    double heading2 = mdmLib.calculateHeadingAngle(nodeHeading2);

    double intFactor2 = mdmLib.EllipseEllipseIntersectionFactor(nodePosition1,
            nodePositionConfidence1, nodePosition2, nodePositionConfidence2,
            heading1, heading2, nodeSize1, nodeSize2);
    intFactor2 = 1 - intFactor2;
    double factor = intFactor2;

    return factor;

}

InterTest CaTChChecks::MultipleIntersectionCheck(NodeTable * detectedNodes,
        BasicSafetyMessage * bsm) {
    unsigned long senderPseudonym = bsm->getSenderPseudonym();
    Coord senderPos = bsm->getSenderPos();
    Coord senderPosConfidence = bsm->getSenderPosConfidence();
    Coord senderHeading = bsm->getSenderHeading();

    Coord senderSize = Coord(bsm->getSenderWidth(), bsm->getSenderLength());

    NodeHistory * senderNode = detectedNodes->getNodeHistoryAddr(
            senderPseudonym);
    NodeHistory * varNode;

    const int maxInterNum = 100;
    unsigned long resultPseudo[maxInterNum];
    double resultCheck[maxInterNum];

    double INTScore = 0;
    int INTNum = 0;

    INTScore = IntersectionCheck(myPosition, myPositionConfidence, senderPos,
            senderPosConfidence, myHeading, senderHeading, mySize, senderSize);
    if (INTScore < 1) {
        resultPseudo[INTNum] = myPseudonym;
        resultCheck[INTNum] = INTScore;
        INTNum++;
    }

    for (int var = 0; var < detectedNodes->getNodesNum(); ++var) {
        if (detectedNodes->getNodePseudo(var) != senderPseudonym) {
            varNode = detectedNodes->getNodeHistoryAddr(
                    detectedNodes->getNodePseudo(var));

            if (mdmLib.calculateDeltaTime(varNode->getLatestBSMAddr(),
                    bsm) < MAX_DELTA_INTER) {

                Coord varSize = Coord(
                        varNode->getLatestBSMAddr()->getSenderWidth(),
                        varNode->getLatestBSMAddr()->getSenderLength());

                INTScore = IntersectionCheck(
                        varNode->getLatestBSMAddr()->getSenderPos(),
                        varNode->getLatestBSMAddr()->getSenderPosConfidence(),
                        senderPos, senderPosConfidence,
                        varNode->getLatestBSMAddr()->getSenderHeading(),
                        senderHeading, varSize, senderSize);
                if (INTScore < 1) {

                    resultPseudo[INTNum] = detectedNodes->getNodePseudo(var);
                    resultCheck[INTNum] = INTScore;

                    INTNum++;
                }
            }
        }
    }

    InterTest intertTest = InterTest(INTNum);

    for (int var = 0; var < INTNum; ++var) {
        intertTest.addInterValue(resultPseudo[var], resultCheck[var]);

    }
    return intertTest;
}

double CaTChChecks::SuddenAppearenceCheck(Coord receiverPosition,
        Coord receiverPositionConfidence, Coord senderPosition,
        Coord senderPositionConfidence) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);
    double r1 = senderPositionConfidence.x;
    double r2 = SUDDEN_APPEARENCE_RANGE + receiverPositionConfidence.x;

    double factor = 0;
    if (r1 <= 0) {
        if (distance
                < (SUDDEN_APPEARENCE_RANGE + receiverPositionConfidence.x)) {
            factor = 0;
        } else {
            factor = 1;
        }
    } else {
        double area = mdmLib.calculateCircleCircleIntersection(r1, r2,
                distance);

        factor = area / (PI * r1 * r1);
        factor = 1 - factor;
    }

    return factor;
}

double CaTChChecks::PositionPlausibilityCheck(Coord senderPosition,
        Coord senderPositionConfidence, double senderSpeed,
        double senderSpeedConfidence) {

    double speedd = senderSpeed - senderSpeedConfidence;
    if (speedd < 0) {
        speedd = 0;
    }

    if (speedd <= MAX_NON_ROUTE_SPEED) {
        return 1;
    }

    double resolution = senderPositionConfidence.x / 10;
    if (resolution < 1) {
        resolution = 1;
    }
    double resolutionDelta = resolution / 10;

    double failedCount = 0;
    double allCount = 1;

    if (LinkC->calculateDistance(senderPosition, 50,
            50) > MAX_DISTANCE_FROM_ROUTE) {
        failedCount++;
    }


    int resolutionTheta = 0;

    for (double r = resolution; r < senderPositionConfidence.x;
            r = r + resolution) {
        resolutionTheta = (int) (2 * PI * r / (resolution));
        //std::cout << r<< "#" << resolution << "^" << resolutionTheta<<"-";
        for (int t = 0; t < resolutionTheta; ++t) {
            Coord p(senderPosition.x + r * cos(2 * PI * t / resolutionTheta),
                    senderPosition.y + r * sin(2 * PI * t / resolutionTheta));

            if (LinkC->calculateDistance(p, 50, 50) > MAX_DISTANCE_FROM_ROUTE) {
                failedCount++;
            }
            allCount++;
        }
        resolution = resolution + resolutionDelta;
    }

    return (1 - (failedCount / allCount));

    double Intersection = 0;
    int count = 5;

    ObstacleControl* obstacles = ObstacleControlAccess().getIfExists();

    Intersection = obstacles->calculateInsersion(senderPosition, resolution / 2,
            resolution / 2);

    for (double r = resolution; r < senderPositionConfidence.x;
            r = r + resolution) {
        int resolutionTheta = (int) (2 * PI * r / (resolution));
        //std::cout << r<< "#" << resolution << "^" << resolutionTheta<<"-";
        Coord pList[resolutionTheta];
        for (int t = 0; t < resolutionTheta; ++t) {
            Coord p(senderPosition.x + r * cos(2 * PI * t / resolutionTheta),
                    senderPosition.y + r * sin(2 * PI * t / resolutionTheta));
            pList[t] = p;
            count++;
        }

        double IntersectionTemp = obstacles->calculateInsersionList(
                senderPosition, r, r, pList, resolutionTheta);

        Intersection = Intersection + IntersectionTemp;

        resolution = resolution + resolutionDelta;
    }
    //return Intersection;

//    if ((1 - (Intersection / count)) < 0.1) {
//        std::cout
//                << "*******************************************************************************"
//                << '\n';
//        std::cout
//                << "*******************************************************************************Intersection: "
//                << Intersection << " count:" << count << '\n';
//        std::cout
//                << "*******************************************************************************"
//                << '\n';
//    }

    double factor = (1 - (Intersection / count));

    return factor;
}

double CaTChChecks::BeaconFrequencyCheck(double timeNew, double timeOld) {
    double timeDelta = timeNew - timeOld;
    if (timeDelta < MAX_BEACON_FREQUENCY) {
        return 0;
    } else {
        return 1;
    }
}

double CaTChChecks::PositionHeadingConsistancyCheck(Coord curHeading,
        Coord curHeadingConfidence, Coord oldPosition,
        Coord oldPositionConfidence, Coord curPosition,
        Coord curPositionConfidence, double deltaTime, double curSpeed,
        double curSpeedConfidence) {
    if (deltaTime < POS_HEADING_TIME) {
        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        if (distance < 1) {
            return 1;
        }

        if (curSpeed - curSpeedConfidence < 1) {
            return 1;
        }

        double curHeadingAngle = mdmLib.calculateHeadingAngle(curHeading);

        Coord relativePos = Coord(curPosition.x - oldPosition.x,
                curPosition.y - oldPosition.y, curPosition.z - oldPosition.z);
        double positionAngle = mdmLib.calculateHeadingAngle(relativePos);
        double angleDelta = fabs(curHeadingAngle - positionAngle);
        if (angleDelta > 180) {
            angleDelta = 360 - angleDelta;
        }

        double angleLow = angleDelta - curHeadingConfidence.x;
        if (angleLow < 0) {
            angleLow = 0;
        }

        double angleHigh = angleDelta + curHeadingConfidence.x;
        if (angleHigh > 180) {
            angleHigh = 180;
        }

        double xLow = distance * cos(angleLow * PI / 180);

        double curFactorLow = 1;
        if (curPositionConfidence.x == 0) {
            if (angleLow <= MAX_HEADING_CHANGE) {
                curFactorLow = 1;
            } else {
                curFactorLow = 0;
            }
        } else {
            curFactorLow = mdmLib.calculateCircleSegment(
                    curPositionConfidence.x, curPositionConfidence.x + xLow)
                    / (PI * curPositionConfidence.x * curPositionConfidence.x);
        }

        double oldFactorLow = 1;
        if (oldPositionConfidence.x == 0) {
            if (angleLow <= MAX_HEADING_CHANGE) {
                oldFactorLow = 1;
            } else {
                oldFactorLow = 0;
            }
        } else {
            oldFactorLow = 1
                    - mdmLib.calculateCircleSegment(oldPositionConfidence.x,
                            oldPositionConfidence.x - xLow)
                            / (PI * oldPositionConfidence.x
                                    * oldPositionConfidence.x);
        }

        double xHigh = distance * cos(angleHigh * PI / 180);
        double curFactorHigh = 1;
        if (curPositionConfidence.x == 0) {
            if (angleHigh <= MAX_HEADING_CHANGE) {
                curFactorHigh = 1;
            } else {
                curFactorHigh = 0;
            }
        } else {
            curFactorHigh = mdmLib.calculateCircleSegment(
                    curPositionConfidence.x, curPositionConfidence.x + xHigh)
                    / (PI * curPositionConfidence.x * curPositionConfidence.x);
        }

        double oldFactorHigh = 1;
        if (oldPositionConfidence.x == 0) {
            if (angleHigh <= MAX_HEADING_CHANGE) {
                oldFactorHigh = 1;
            } else {
                oldFactorHigh = 0;
            }
        } else {
            oldFactorHigh = 1
                    - mdmLib.calculateCircleSegment(oldPositionConfidence.x,
                            oldPositionConfidence.x - xHigh)
                            / (PI * oldPositionConfidence.x
                                    * oldPositionConfidence.x);
        }

        double factor = (curFactorLow + oldFactorLow + curFactorHigh
                + oldFactorHigh) / 4;

//    if(factor<=0.0){
//
//        std::cout<<"curPos: "<<curPosition<<'\n';
//        std::cout<<"oldPos: "<<oldPosition<<'\n';
//
//        std::cout<<"relativePos: "<<relativePos<<'\n';
//
//        std::cout<<"curFactorLow: "<<curFactorLow<<'\n';
//        std::cout<<"oldFactorLow: "<<oldFactorLow<<'\n';
//        std::cout<<"curFactorHigh: "<<curFactorHigh<<'\n';
//        std::cout<<"oldFactorHigh: "<<oldFactorHigh<<'\n';
//        std::cout<<"positionAngle: "<<positionAngle<<'\n';
//        std::cout<<"curHeadingAngle: "<<curHeadingAngle<<'\n';
//        std::cout<<"angleDelta: "<<angleDelta<<'\n';
//        std::cout<<"distance: "<<distance<<'\n';
//        std::cout<<"distance: "<<distance<<'\n';
//        std::cout<<"xLow: "<<xLow<<'\n';
//        if(factor == 0){
//            std::cout<<"ZERO: "<<factor<<'\n';
//        }else{
//            std::cout<<"NONZ: "<<factor<<'\n';
//        }
//
//     //   exit(0);
//    }

//        factor = (factor - 0.5) * 2;
//        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
//        if (factor > 0.75) {
//            factor = 1;
//        }
//        if (factor <0.001) {
//            factor = 0;
//        }

        return factor;
    } else {
        return 1;
    }
}

BsmCheck CaTChChecks::CheckBSM(BasicSafetyMessage * bsm,
        NodeTable * detectedNodes) {

    BsmCheck bsmCheck = BsmCheck();

    unsigned long senderPseudonym = bsm->getSenderPseudonym();
    Coord senderPos = bsm->getSenderPos();
    Coord senderPosConfidence = bsm->getSenderPosConfidence();

    NodeHistory * senderNode = detectedNodes->getNodeHistoryAddr(
            senderPseudonym);

    bsmCheck.setRangePlausibility(
            RangePlausibilityCheck(myPosition, myPositionConfidence, senderPos,
                    senderPosConfidence));

    bsmCheck.setSpeedPlausibility(
            SpeedPlausibilityCheck(mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                    mdmLib.calculateSpeed(bsm->getSenderSpeedConfidence())));

    if (detectedNodes->getNodeHistoryAddr(senderPseudonym)->getBSMNum() > 0) {
        bsmCheck.setPositionConsistancy(
                PositionConsistancyCheck(senderPos, senderPosConfidence,
                        senderNode->getLatestBSMAddr()->getSenderPos(),
                        senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr())));

        bsmCheck.setSpeedConsistancy(
                SpeedConsistancyCheck(
                        mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                        mdmLib.calculateSpeed(bsm->getSenderSpeedConfidence()),
                        mdmLib.calculateSpeed(
                                senderNode->getLatestBSMAddr()->getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode->getLatestBSMAddr()->getSenderSpeedConfidence()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr())));

        bsmCheck.setPositionSpeedConsistancy(
                PositionSpeedConsistancyCheck(senderPos, senderPosConfidence,
                        senderNode->getLatestBSMAddr()->getSenderPos(),
                        senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                        mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                        mdmLib.calculateSpeed(bsm->getSenderSpeedConfidence()),
                        mdmLib.calculateSpeed(
                                senderNode->getLatestBSMAddr()->getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode->getLatestBSMAddr()->getSenderSpeedConfidence()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr())));

        bsmCheck.setBeaconFrequency(
                BeaconFrequencyCheck(bsm->getArrivalTime().dbl(),
                        senderNode->getLatestBSMAddr()->getArrivalTime().dbl()));

        bsmCheck.setPositionHeadingConsistancy(
                PositionHeadingConsistancyCheck(bsm->getSenderHeading(),
                        bsm->getSenderHeadingConfidence(),
                        senderNode->getLatestBSMAddr()->getSenderPos(),
                        senderNode->getLatestBSMAddr()->getSenderPosConfidence(),
                        bsm->getSenderPos(), bsm->getSenderPosConfidence(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr()),
                        mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                bsm->getSenderSpeedConfidence())));

    } else {
        bsmCheck.setSuddenAppearence(
                SuddenAppearenceCheck(senderPos, senderPosConfidence,
                        myPosition, myPositionConfidence));
    }

    bsmCheck.setIntersection(MultipleIntersectionCheck(detectedNodes, bsm));



    bsmCheck.setPositionPlausibility(
            PositionPlausibilityCheck(senderPos, senderPosConfidence,
                    mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                    mdmLib.calculateSpeed(bsm->getSenderSpeedConfidence())));

//    PrintBsmCheck(senderPseudonym, bsmCheck);

    return bsmCheck;
}

void CaTChChecks::PrintBsmCheck(unsigned long senderPseudonym,
        BsmCheck bsmCheck) {

    if (bsmCheck.getRangePlausibility() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "ART FAILED => "
                << bsmCheck.getRangePlausibility() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getPositionConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MGTD FAILED => "
                << bsmCheck.getPositionConsistancy() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getSpeedConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MGTS FAILED => "
                << bsmCheck.getSpeedConsistancy() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getPositionSpeedConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MGTSV FAILED => "
                << bsmCheck.getPositionSpeedConsistancy() << " A:"
                << myPseudonym << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getSpeedPlausibility() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MAXS FAILED => "
                << bsmCheck.getSpeedPlausibility() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }
    if (bsmCheck.getPositionPlausibility() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MAP FAILED => "
                << bsmCheck.getPositionPlausibility() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getSuddenAppearence() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "SAW FAILED => "
                << bsmCheck.getSuddenAppearence() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getPositionHeadingConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "PHC FAILED => "
                << bsmCheck.getPositionHeadingConsistancy() << " A:"
                << myPseudonym << " B:" << senderPseudonym << '\n';
    }

    if (bsmCheck.getBeaconFrequency() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "FREQ FAILED => "
                << bsmCheck.getBeaconFrequency() << " A:" << myPseudonym
                << " B:" << senderPseudonym << '\n';
    }

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        if (inter.getInterValue(var) < 0.5) {
            std::cout << "^^^^^^^^^^^V2 " << "INT FAILED => "
                    << inter.getInterValue(var) << " A:" << myPseudonym << " B:"
                    << senderPseudonym << " C:" << inter.getInterId(var)
                    << '\n';
        }
    }

}

