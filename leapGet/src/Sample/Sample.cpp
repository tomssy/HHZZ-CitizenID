/******************************************************************************\
* Copyright (C) 2012-2016 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <xstring>
#include "Leap.h"
#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#define DISPLAY_IDUSER true
#define DISPLAY_GENDER true
#define DISPLAY_HAND true
#define DISPLAY_ARM true
#define DISPLAY_FINGER true
#define DISPLAY_BONE_ST_POS true
#define DISPLAY_BONE_ED_POS true
#define DISPLAY_BONE_DIR true
// #include "handrecord.hpp"

using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
    virtual void onServiceChange(const Controller&);
    virtual void onDeviceFailure(const Controller&);
    virtual void onLogMessage(const Controller&, MessageSeverity severity, int64_t timestamp, const char* msg);
};

int64_t lastTime = 0;

class HandRecord{
  public:
    HandRecord(){
      this->pFingerId[0] = &this->thumbId;
      this->pFingerId[1] = &this->indexId;
      this->pFingerId[2] = &this->middleId;
      this->pFingerId[3] = &this->ringId;
      this->pFingerId[4] = &this->pinkyId;

      this->pFingerBase[0][0] = &this->thumbLen;
      this->pFingerBase[0][1] = &this->thumbWid;
      this->pFingerBase[1][0] = &this->indexLen;
      this->pFingerBase[1][1] = &this->indexWid;
      this->pFingerBase[2][0] = &this->middleLen;
      this->pFingerBase[2][1] = &this->middleWid;
      this->pFingerBase[3][0] = &this->ringLen;
      this->pFingerBase[3][1] = &this->ringWid;
      this->pFingerBase[4][0] = &this->pinkyLen;
      this->pFingerBase[4][1] = &this->pinkyWid;

      this->pFingerParam[0][0][0] = &this->thumbMetStPos;
      this->pFingerParam[0][0][1] = &this->thumbMetEdPos;
      this->pFingerParam[0][0][2] = &this->thumbMetDir;
      this->pFingerParam[0][1][0] = &this->thumbProStPos;
      this->pFingerParam[0][1][1] = &this->thumbProEdPos;
      this->pFingerParam[0][1][2] = &this->thumbProDir;
      this->pFingerParam[0][2][0] = &this->thumbMidStPos;
      this->pFingerParam[0][2][1] = &this->thumbMidEdPos;
      this->pFingerParam[0][2][2] = &this->thumbMidDir;
      this->pFingerParam[0][3][0] = &this->thumbDisStPos;
      this->pFingerParam[0][3][1] = &this->thumbDisEdPos;
      this->pFingerParam[0][3][2] = &this->thumbDisDir;

      this->pFingerParam[1][0][0] = &this->indexMetStPos;
      this->pFingerParam[1][0][1] = &this->indexMetEdPos;
      this->pFingerParam[1][0][2] = &this->indexMetDir;
      this->pFingerParam[1][1][0] = &this->indexProStPos;
      this->pFingerParam[1][1][1] = &this->indexProEdPos;
      this->pFingerParam[1][1][2] = &this->indexProDir;
      this->pFingerParam[1][2][0] = &this->indexMidStPos;
      this->pFingerParam[1][2][1] = &this->indexMidEdPos;
      this->pFingerParam[1][2][2] = &this->indexMidDir;
      this->pFingerParam[1][3][0] = &this->indexDisStPos;
      this->pFingerParam[1][3][1] = &this->indexDisEdPos;
      this->pFingerParam[1][3][2] = &this->indexDisDir;

      this->pFingerParam[2][0][0] = &this->middleMetStPos;
      this->pFingerParam[2][0][1] = &this->middleMetEdPos;
      this->pFingerParam[2][0][2] = &this->middleMetDir;
      this->pFingerParam[2][1][0] = &this->middleProStPos;
      this->pFingerParam[2][1][1] = &this->middleProEdPos;
      this->pFingerParam[2][1][2] = &this->middleProDir;
      this->pFingerParam[2][2][0] = &this->middleMidStPos;
      this->pFingerParam[2][2][1] = &this->middleMidEdPos;
      this->pFingerParam[2][2][2] = &this->middleMidDir;
      this->pFingerParam[2][3][0] = &this->middleDisStPos;
      this->pFingerParam[2][3][1] = &this->middleDisEdPos;
      this->pFingerParam[2][3][2] = &this->middleDisDir;

      this->pFingerParam[3][0][0] = &this->ringMetStPos;
      this->pFingerParam[3][0][1] = &this->ringMetEdPos;
      this->pFingerParam[3][0][2] = &this->ringMetDir;
      this->pFingerParam[3][1][0] = &this->ringProStPos;
      this->pFingerParam[3][1][1] = &this->ringProEdPos;
      this->pFingerParam[3][1][2] = &this->ringProDir;
      this->pFingerParam[3][2][0] = &this->ringMidStPos;
      this->pFingerParam[3][2][1] = &this->ringMidEdPos;
      this->pFingerParam[3][2][2] = &this->ringMidDir;
      this->pFingerParam[3][3][0] = &this->ringDisStPos;
      this->pFingerParam[3][3][1] = &this->ringDisEdPos;
      this->pFingerParam[3][3][2] = &this->ringDisDir;

      this->pFingerParam[4][0][0] = &this->pinkyMetStPos;
      this->pFingerParam[4][0][1] = &this->pinkyMetEdPos;
      this->pFingerParam[4][0][2] = &this->pinkyMetDir;
      this->pFingerParam[4][1][0] = &this->pinkyProStPos;
      this->pFingerParam[4][1][1] = &this->pinkyProEdPos;
      this->pFingerParam[4][1][2] = &this->pinkyProDir;
      this->pFingerParam[4][2][0] = &this->pinkyMidStPos;
      this->pFingerParam[4][2][1] = &this->pinkyMidEdPos;
      this->pFingerParam[4][2][2] = &this->pinkyMidDir;
      this->pFingerParam[4][3][0] = &this->pinkyDisStPos;
      this->pFingerParam[4][3][1] = &this->pinkyDisEdPos;
      this->pFingerParam[4][3][2] = &this->pinkyDisDir;

    }

    int8_t id;
    char userName[32];
    char gender;

    int64_t frameId;
    int64_t frameTimestamp;

    int32_t handId;
    bool handIsRight;
    Vector handPalmPos;
    float handPitch, handRoll, handYaw;

    Vector armDir;
    Vector wristPos;
    Vector elbowPos;

    int32_t * pFingerId[5];
    float * pFingerBase[5][2];
    Vector * pFingerParam[5][4][3];

    int32_t thumbId;
    float thumbLen;
    float thumbWid;
    Vector thumbMetStPos;
    Vector thumbMetEdPos;
    Vector thumbMetDir;
    Vector thumbProStPos;
    Vector thumbProEdPos;
    Vector thumbProDir;
    Vector thumbMidStPos;
    Vector thumbMidEdPos;
    Vector thumbMidDir;
    Vector thumbDisStPos;
    Vector thumbDisEdPos;
    Vector thumbDisDir;

    int32_t indexId;
    float indexLen;
    float indexWid;
    Vector indexMetStPos;
    Vector indexMetEdPos;
    Vector indexMetDir;
    Vector indexProStPos;
    Vector indexProEdPos;
    Vector indexProDir;
    Vector indexMidStPos;
    Vector indexMidEdPos;
    Vector indexMidDir;
    Vector indexDisStPos;
    Vector indexDisEdPos;
    Vector indexDisDir;

    int32_t middleId;
    float middleLen;
    float middleWid;
    Vector middleMetStPos;
    Vector middleMetEdPos;
    Vector middleMetDir;
    Vector middleProStPos;
    Vector middleProEdPos;
    Vector middleProDir;
    Vector middleMidStPos;
    Vector middleMidEdPos;
    Vector middleMidDir;
    Vector middleDisStPos;
    Vector middleDisEdPos;
    Vector middleDisDir;

    int32_t ringId;
    float ringLen;
    float ringWid;
    Vector ringMetStPos;
    Vector ringMetEdPos;
    Vector ringMetDir;
    Vector ringProStPos;
    Vector ringProEdPos;
    Vector ringProDir;
    Vector ringMidStPos;
    Vector ringMidEdPos;
    Vector ringMidDir;
    Vector ringDisStPos;
    Vector ringDisEdPos;
    Vector ringDisDir;

    int32_t pinkyId;
    float pinkyLen;
    float pinkyWid;
    Vector pinkyMetStPos;
    Vector pinkyMetEdPos;
    Vector pinkyMetDir;
    Vector pinkyProStPos;
    Vector pinkyProEdPos;
    Vector pinkyProDir;
    Vector pinkyMidStPos;
    Vector pinkyMidEdPos;
    Vector pinkyMidDir;
    Vector pinkyDisStPos;
    Vector pinkyDisEdPos;
    Vector pinkyDisDir;
};

HandRecord rds[5];

int8_t rCtl = 0;
int8_t rId = 0;
bool reading = false;
bool writeing = true;

TCHAR ss[256];
TCHAR ssEmpty[256] = _T("                                                                                                                                                                ");


const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};

#define WINDOWW 1024
#define WINDOWH 600
#define OFFSET(x, y) x + 800 , WINDOWH - y - 300
#define SETCOLOR(x) settextcolor(x); setlinecolor(x);

void printCommand(void){
    settextcolor(LIGHTGRAY);
    /*
    outtextxy(WINDOWW / 2, WINDOWH - 16, ssEmpty);
    outtextxy(WINDOWW / 2, WINDOWH - 16, ss);
    */
    outtextxy(0, 700, ssEmpty);
    outtextxy(0, 700, ss);
    Sleep(100);
    return;
}

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void perfectPostion(int ind){
    float yAvg = 0, yVan = 0;
    for(int fingerType = 0; fingerType < 5; fingerType++){
        for(int boneType = 0; boneType < 4; boneType++){
            yAvg += (*rds[ind].pFingerParam[fingerType][boneType][1]).y;
        }
    }
    yAvg = yAvg / 20.0;
    for(int fingerType = 0; fingerType < 5; fingerType++){
        for(int boneType = 0; boneType < 4; boneType++){
            yVan += pow(yAvg - (*rds[ind].pFingerParam[fingerType][boneType][1]).y, 2);
        }
    }

    if((yAvg > 140) && (yAvg < 160) && (yVan < 2500)){
      setlinecolor(GREEN);
      setfillcolor(GREEN);
      POINT pts[] = {{0, WINDOWH - 100}, {WINDOWW, WINDOWH - 100}, {WINDOWW, WINDOWH}, {0, WINDOWH}};
      solidpolygon(pts, 4);
      settextcolor(BLACK);
      settextstyle(100, 0, _T("宋体"));
      outtextxy(320, WINDOWH - 100, _T("Perfect"));
      graphdefaults();
      setbkcolor(WHITE);
    }else{
      setlinecolor(LIGHTGRAY);
      setfillcolor(LIGHTGRAY);
      POINT pts[] = {{0, WINDOWH - 100}, {WINDOWW, WINDOWH - 100}, {WINDOWW, WINDOWH}, {0, WINDOWH}};
      solidpolygon(pts, 4);
      settextcolor(RED);
        TCHAR yAvgStr[256];
        TCHAR yVanStr[256];

        _stprintf(yAvgStr, _T("yAvg = %f"), yAvg);
        _stprintf(yVanStr, _T("yVan = %f"), yVan);

        outtextxy(512 - 100, WINDOWH - 84, yAvgStr);
        outtextxy(512 - 100, WINDOWH - 84 + 16, yVanStr);
        outtextxy(512 - 100, WINDOWH - 84 + 32, _T("Make yAvg between 140 and 160"));
        outtextxy(512 - 100, WINDOWH - 84 + 48, _T("Make yVan small than 2500"));
    }

    return;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  // std::cout << "help" << std::endl;
  // std::cout << (frame.timestamp() - lastTime) << std::endl;
  if((frame.timestamp() - lastTime > 100000) && !reading){
    lastTime = frame.timestamp();
    // std::cout << "writeing" << std::endl;
    writeing = true;
    // write a record
    rds[rCtl].id = rId;
    strcat(rds[rCtl].userName, " ");
    rds[rCtl].frameId = frame.id();
    rds[rCtl].frameTimestamp = frame.timestamp();
    HandList hands = frame.hands();
    bool recorded = false;
    for(HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl){
      const Hand hand = *hl;
      if(recorded && hand.isRight()){
        break;
      }
      rds[rCtl].handId = hand.id();
      rds[rCtl].handIsRight = hand.isRight();
      rds[rCtl].handPalmPos = hand.palmPosition();
      cleardevice();
      TCHAR s[256];
      _stprintf_s(s, _T("handId: %d, %s hand"), rds[rCtl].handId, rds[rCtl].handIsRight?_T("right"):_T("left"));
      settextcolor(DARKGRAY);
      outtextxy(0, 0, s);
      _stprintf_s(s, _T("handPalmPos: %f, %f"), rds[rCtl].handPalmPos.x, rds[rCtl].handPalmPos.z);
      outtextxy(0, 16, s);
      setlinecolor(DARKGRAY);
      // circle(OFFSET(rds[rCtl].handPalmPos.x * 2, rds[rCtl].handPalmPos.z) * (-2), 9);
      const Vector normail = hand.palmNormal();
      const Vector direction = hand.direction();
      rds[rCtl].handPitch = direction.pitch() * RAD_TO_DEG;
      rds[rCtl].handRoll = normail.roll() * RAD_TO_DEG;
      rds[rCtl].handYaw = direction.yaw() * RAD_TO_DEG;
      Arm arm = hand.arm();
      rds[rCtl].armDir = arm.direction();
      rds[rCtl].wristPos = arm.wristPosition();
      rds[rCtl].elbowPos = arm.elbowPosition();
      FingerList fingers = hand.fingers();
      for(FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl){
        const Finger finger = *fl;
        Finger::Type fingerType = finger.type();
        *rds[rCtl].pFingerId[fingerType] = finger.id();
        *rds[rCtl].pFingerBase[fingerType][0] = finger.length();
        *rds[rCtl].pFingerBase[fingerType][1] = finger.width();
        TCHAR fingerName[32];
        switch(fingerType){
            case 0: _stprintf_s(fingerName, _T("%s"), _T("thumb")); SETCOLOR(LIGHTBLUE); break;
            case 1: _stprintf_s(fingerName, _T("%s"), _T("index")); SETCOLOR(LIGHTGRAY); break;
            case 2: _stprintf_s(fingerName, _T("%s"), _T("middle")); SETCOLOR(BROWN); break;
            case 3: _stprintf_s(fingerName, _T("%s"), _T("ring")); SETCOLOR(LIGHTRED); break;
            case 4: _stprintf_s(fingerName, _T("%s"), _T("pinky")); SETCOLOR(LIGHTMAGENTA); break;
            default : break;
        }
        _stprintf_s(s, _T("    %s finger, length = %f, width = %f"), fingerName,
                *rds[rCtl].pFingerBase[fingerType][0],
                *rds[rCtl].pFingerBase[fingerType][1]
                );
        outtextxy(0, 16 + 16 + 80 * fingerType, s);
        // _stprintf_s_s();
        for(int b = 0; b < 4; ++b){
          Bone::Type boneType = static_cast<Bone::Type>(b);
          Bone bone = finger.bone(boneType);
          *rds[rCtl].pFingerParam[fingerType][boneType][0] = bone.prevJoint();
          *rds[rCtl].pFingerParam[fingerType][boneType][1] = bone.nextJoint();
          *rds[rCtl].pFingerParam[fingerType][boneType][2] = bone.direction();
          TCHAR boneName[32];
          switch(b){
              case 0: _stprintf_s(boneName, _T("%s"), _T("metacarpal")); SETCOLOR(BROWN); break;
              case 1: _stprintf_s(boneName, _T("%s"), _T("proximal")); SETCOLOR(GREEN); break;
              case 2: _stprintf_s(boneName, _T("%s"), _T("intermediate")); SETCOLOR(BLUE); break;
              case 3: _stprintf_s(boneName, _T("%s"), _T("distal")); SETCOLOR(RED); break;
              default : break;
          }
          _stprintf_s(s, _T("        %s bone: (%f, %f)->(%f, %f)"), boneName,
                  bone.prevJoint().x,
                  bone.prevJoint().z,
                  bone.nextJoint().x,
                  bone.nextJoint().z
                  );
          outtextxy(0, 16 + 16 + 80 * fingerType + 16 + 16 * boneType, s);
          circle(OFFSET(bone.prevJoint().x * 2, bone.prevJoint().z * (-2)), 3);
          circle(OFFSET(bone.nextJoint().x * 2, bone.nextJoint().z * (-2)), 3);
          line(OFFSET(bone.prevJoint().x * 2, bone.prevJoint().z * (-2)), OFFSET(bone.nextJoint().x * 2, bone.nextJoint().z * (-2)));
          perfectPostion(rCtl);
        }
      }
    }
      EndBatchDraw();
      BeginBatchDraw();
    writeing = false;
    // std::cout << "write done" << std::endl;
    // loop step
    rCtl = ++rCtl % 5;
  }

}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    std::cout << "  isSmudged:" << (devices[i].isSmudged() ? "true" : "false") << std::endl;
    std::cout << "  isLightingBad:" << (devices[i].isLightingBad() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

void SampleListener::onServiceChange(const Controller& controller) {
  std::cout << "Service Changed" << std::endl;
}

void SampleListener::onDeviceFailure(const Controller& controller) {
  std::cout << "Device Error" << std::endl;
  const Leap::FailedDeviceList devices = controller.failedDevices();

  for (FailedDeviceList::const_iterator dl = devices.begin(); dl != devices.end(); ++dl) {
    const FailedDevice device = *dl;
    std::cout << "  PNP ID:" << device.pnpId();
    std::cout << "    Failure type:" << device.failure();
  }
}

void SampleListener::onLogMessage(const Controller&, MessageSeverity s, int64_t t, const char* msg) {
  switch (s) {
  case Leap::MESSAGE_CRITICAL:
    std::cout << "[Critical]";
    break;
  case Leap::MESSAGE_WARNING:
    std::cout << "[Warning]";
    break;
  case Leap::MESSAGE_INFORMATION:
    std::cout << "[Info]";
    break;
  case Leap::MESSAGE_UNKNOWN:
    std::cout << "[Unknown]";
  }
  std::cout << "[" << t << "] ";
  std::cout << msg << std::endl;
}

int main(int argc, char** argv) {
    // 测试easyx
    initgraph(WINDOWW, WINDOWH, SHOWCONSOLE | NOCLOSE);
    setorigin(0, 0);
    setbkcolor(WHITE);
    BeginBatchDraw();
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  controller.setPolicy(Leap::Controller::POLICY_ALLOW_PAUSE_RESUME);

  // Keep this process running until Enter is pressed
  std::cout << "Enter 'q' to quit, or enter 'p' to pause or 'm' to save a record " << std::endl;

  std::ofstream out;
  std::ofstream features;
  features.open("handfeatures.csv", std::ios::app);
  out.open("handrecord.csv", std::ios::app);
  if(features.is_open()){
      std::cout << "Success to open the handfeares.csv" << std::endl;
      features << "id,handWidth,thumbLength,indexLength,middleLength" << std::endl;
      features.close();
  }else{
      std::cout << "Failed to open the handfeatures.csv, the command 'm' and 'f' while not work" << std::endl;
  }
  if(out.is_open()){
    std::cout << "Success to open the handrecord.csv" << std::endl;
    if(DISPLAY_IDUSER)
        out << "id,user_name,";
    if(DISPLAY_GENDER)
        out << "gender,";
    out << "frame_id,frame_timestamp,";
    if(DISPLAY_HAND){
       out << "hand_id,hand_is_right,";
        out << "hand_palm_pos_x,hand_palm_pos_y,hand_palm_pos_z,";
        out << "hand_pitch,hand_roll,hand_yaw,";
    }
    if(DISPLAY_ARM){
        out << "arm_dir_x,arm_dir_y,arm_dir_z,";
        out << "wrist_pos_x,wrist_pos_y,wrist_pos_z,";
        out << "elbow_pos_x,elbow_pos_y,elbow_pos_z,";
    }
    if(DISPLAY_FINGER)
        out << "thumb_id,thumb_len,thumb_wid,";
    if(DISPLAY_BONE_ST_POS)
        out << "thumb_met_st_pos_x,thumb_met_st_pos_y,thumb_met_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "thumb_met_ed_pos_x,thumb_met_ed_pos_y,thumb_met_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "thumb_met_dir_x,thumb_met_dir_y,thumb_met_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "thumb_pro_st_pos_x,thumb_pro_st_pos_y,thumb_pro_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "thumb_pro_ed_pos_x,thumb_pro_ed_pos_y,thumb_pro_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "thumb_pro_dir_x,thumb_pro_dir_y,thumb_pro_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "thumb_mid_st_pos_x,thumb_mid_st_pos_y,thumb_mid_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "thumb_mid_ed_pos_x,thumb_mid_ed_pos_y,thumb_mid_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "thumb_mid_dir_x,thumb_mid_dir_y,thumb_mid_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "thumb_dis_st_pos_x,thumb_dis_st_pos_y,thumb_dis_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "thumb_dis_ed_pos_x,thumb_dis_ed_pos_y,thumb_dis_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "thumb_dis_dir_x,thumb_dis_dir_y,thumb_dis_dir_z,";
    if(DISPLAY_FINGER)
        out << "index_id,index_len,index_wid,";
    if(DISPLAY_BONE_ST_POS)
        out << "index_met_st_pos_x,index_met_st_pos_y,index_met_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "index_met_ed_pos_x,index_met_ed_pos_y,index_met_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "index_met_dir_x,index_met_dir_y,index_met_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "index_pro_st_pos_x,index_pro_st_pos_y,index_pro_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "index_pro_ed_pos_x,index_pro_ed_pos_y,index_pro_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "index_pro_dir_x,index_pro_dir_y,index_pro_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "index_mid_st_pos_x,index_mid_st_pos_y,index_mid_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "index_mid_ed_pos_x,index_mid_ed_pos_y,index_mid_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "index_mid_dir_x,index_mid_dir_y,index_mid_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "index_dis_st_pos_x,index_dis_st_pos_y,index_dis_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "index_dis_ed_pos_x,index_dis_ed_pos_y,index_dis_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "index_dis_dir_x,index_dis_dir_y,index_dis_dir_z,";
    if(DISPLAY_FINGER)
        out << "middle_id,middle_len,middle_wid,";
    if(DISPLAY_BONE_ST_POS)
        out << "middle_met_st_pos_x,middle_met_st_pos_y,middle_met_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "middle_met_ed_pos_x,middle_met_ed_pos_y,middle_met_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "middle_met_dir_x,middle_met_dir_y,middle_met_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "middle_pro_st_pos_x,middle_pro_st_pos_y,middle_pro_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "middle_pro_ed_pos_x,middle_pro_ed_pos_y,middle_pro_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "middle_pro_dir_x,middle_pro_dir_y,middle_pro_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "middle_mid_st_pos_x,middle_mid_st_pos_y,middle_mid_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "middle_mid_ed_pos_x,middle_mid_ed_pos_y,middle_mid_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "middle_mid_dir_x,middle_mid_dir_y,middle_mid_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "middle_dis_st_pos_x,middle_dis_st_pos_y,middle_dis_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "middle_dis_ed_pos_x,middle_dis_ed_pos_y,middle_dis_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "middle_dis_dir_x,middle_dis_dir_y,middle_dis_dir_z,";
    if(DISPLAY_FINGER)
        out << "ring_id,ring_len,ring_wid,";
    if(DISPLAY_BONE_ST_POS)
        out << "ring_met_st_pos_x,ring_met_st_pos_y,ring_met_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "ring_met_ed_pos_x,ring_met_ed_pos_y,ring_met_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "ring_met_dir_x,ring_met_dir_y,ring_met_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "ring_pro_st_pos_x,ring_pro_st_pos_y,ring_pro_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "ring_pro_ed_pos_x,ring_pro_ed_pos_y,ring_pro_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "ring_pro_dir_x,ring_pro_dir_y,ring_pro_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "ring_mid_st_pos_x,ring_mid_st_pos_y,ring_mid_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "ring_mid_ed_pos_x,ring_mid_ed_pos_y,ring_mid_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "ring_mid_dir_x,ring_mid_dir_y,ring_mid_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "ring_dis_st_pos_x,ring_dis_st_pos_y,ring_dis_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "ring_dis_ed_pos_x,ring_dis_ed_pos_y,ring_dis_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "ring_dis_dir_x,ring_dis_dir_y,ring_dis_dir_z,";
    if(DISPLAY_FINGER)
        out << "pinky_id,pinky_len,pinky_wid,";
    if(DISPLAY_BONE_ST_POS)
        out << "pinky_met_st_pos_x,pinky_met_st_pos_y,pinky_met_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "pinky_met_ed_pos_x,pinky_met_ed_pos_y,pinky_met_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "pinky_met_dir_x,pinky_met_dir_y,pinky_met_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "pinky_pro_st_pos_x,pinky_pro_st_pos_y,pinky_pro_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "pinky_pro_ed_pos_x,pinky_pro_ed_pos_y,pinky_pro_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "pinky_pro_dir_x,pinky_pro_dir_y,pinky_pro_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "pinky_mid_st_pos_x,pinky_mid_st_pos_y,pinky_mid_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "pinky_mid_ed_pos_x,pinky_mid_ed_pos_y,pinky_mid_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "pinky_mid_dir_x,pinky_mid_dir_y,pinky_mid_dir_z,";
    if(DISPLAY_BONE_ST_POS)
        out << "pinky_dis_st_pos_x,pinky_dis_st_pos_y,pinky_dis_st_pos_z,";
    if(DISPLAY_BONE_ED_POS)
        out << "pinky_dis_ed_pos_x,pinky_dis_ed_pos_y,pinky_dis_ed_pos_z,";
    if(DISPLAY_BONE_DIR)
        out << "pinky_dis_dir_x,pinky_dis_dir_y,pinky_dis_dir_z";
    out << std::endl;
    out.close();
  }else{
    std::cout << "Failed to open the handrecord.csv, the command 'm' and 'f' while not work" << std::endl;
  }

  bool paused = false;
  while (true) {
    // char c = std::cin._getch();
    char c = _getch();
    if (c == 'p') {
      paused = !paused;
      controller.setPaused(paused);
    //   std::cin.get(); //skip the newline
    }else if((c == 'm') || (c == 'f')){
      std::cout << "Record for No." << std::to_string(rId) << "\tGender." << ((c == 'm')?("male"):("female")) << std::endl;
      _stprintf_s(ss, _T("Record for No.%d Gender.%s"), rId, (c == 'm')?_T("male"):_T("female"));
      printCommand();
        reading = true;
      if(writeing){
        std::cout << "Appending" << std::endl;
        _stprintf_s(ss, _T("Appending"));
        printCommand();
        for(int i = 0; i < 1000000000000; i++);
      }else{
        std::cout << "Appending" << std::endl;
        _stprintf_s(ss, _T("Appending"));
        printCommand();
      }
        for(int line = 0; line < 5; line++){
            features.open("handfeatures.csv", std::ios::app);
            if(features.is_open()){
                float handWidth = sqrt(
                        pow(rds[line].indexProStPos.x - rds[line].pinkyProStPos.x, 2) + 
                        pow(rds[line].indexProStPos.y - rds[line].pinkyProStPos.y, 2) +
                        pow(rds[line].indexProStPos.z - rds[line].pinkyProStPos.z, 2)
                        ) + (rds[line].indexWid + rds[line].pinkyWid) / 2.0;
                float fingerLen[3] = {0, 0, 0};
                for(int fingerType = 0; fingerType < 3; fingerType++){
                    for(int boneType = fingerType?1:2; boneType < 4; boneType++){
                        fingerLen[fingerType] += sqrt(
                                pow((*rds[line].pFingerParam[fingerType][boneType][0]).x - 
                                    (*rds[line].pFingerParam[fingerType][boneType][1]).x, 2) +
                                pow((*rds[line].pFingerParam[fingerType][boneType][0]).y -
                                    (*rds[line].pFingerParam[fingerType][boneType][1]).y, 2) +
                                pow((*rds[line].pFingerParam[fingerType][boneType][0]).z -
                                    (*rds[line].pFingerParam[fingerType][boneType][1]).z, 2)
                                );
                    }
                }
                features << std::to_string(rId) << ",";
                features << std::to_string(handWidth) << ",";
                features << fingerLen[0] << ",";
                features << fingerLen[1] << ",";
                features << fingerLen[2] << std::endl;
                features.close();
            }else{
                std::cout << "Failed to open the handfeatures.csv, the command 'm' while not work" << std::endl;
            }
      out.open("handrecord.csv", std::ios::app);
      if(out.is_open()){
          // write
          if(DISPLAY_IDUSER){
            out << std::to_string(rId) << ",";                      // A
            out << ",";                                             // B
          }
          if(DISPLAY_GENDER){
            if(c == 'm'){
                rds[line].gender = 'm';
            }else if(c == 'w'){
                rds[line].gender = 'w';
            }
            out << rds[line].gender << ",";
          }
          out << rds[line].frameId << ",";                        // C
          out << rds[line].frameTimestamp << ",";                 // D
          if(DISPLAY_HAND){
              out << rds[line].handId << ",";                         // E
              out << rds[line].handIsRight << ",";                    // F
              out << rds[line].handPalmPos.x << ",";                  // G
              out << rds[line].handPalmPos.y << ",";                  // H
              out << rds[line].handPalmPos.z << ",";                  // I
              out << rds[line].handPitch << ",";                      // J
              out << rds[line].handRoll << ",";                       // K
              out << rds[line].handYaw << ",";                        // L
          }
          if(DISPLAY_ARM){
              out << rds[line].armDir.x << ",";                       // M
              out << rds[line].armDir.y << ",";                       // N
              out << rds[line].armDir.z << ",";                       // O
              out << rds[line].wristPos.x << ",";                     // P
              out << rds[line].wristPos.y << ",";                     // Q
              out << rds[line].wristPos.z << ",";                     // R
              out << rds[line].elbowPos.x << ",";                     // S
              out << rds[line].elbowPos.y << ",";                     // T
              out << rds[line].elbowPos.z << ",";                     // U
          }
          for(int fingerType = 0; fingerType < 5; fingerType++){
            if(DISPLAY_FINGER){
                out << *rds[line].pFingerBase[fingerType][0] << ",";
                out << *rds[line].pFingerBase[fingerType][1] << ",";
                out << *rds[line].pFingerBase[fingerType][2] << ",";
            }
            for(int boneType = 0; boneType < 4; boneType++){
              if(DISPLAY_BONE_ST_POS){
                  out << (*rds[line].pFingerParam[fingerType][boneType][0]).x << ",";
                  out << (*rds[line].pFingerParam[fingerType][boneType][0]).y << ",";
                  out << (*rds[line].pFingerParam[fingerType][boneType][0]).z << ",";
              }
              if(DISPLAY_BONE_ED_POS){
                  out << (*rds[line].pFingerParam[fingerType][boneType][1]).x << ",";
                  out << (*rds[line].pFingerParam[fingerType][boneType][1]).y << ",";
                  out << (*rds[line].pFingerParam[fingerType][boneType][1]).z << ",";
              }
              if(DISPLAY_BONE_DIR){
                  out << (*rds[line].pFingerParam[fingerType][boneType][2]).x << ",";
                  out << (*rds[line].pFingerParam[fingerType][boneType][2]).y << ",";
                  out << (*rds[line].pFingerParam[fingerType][boneType][2]).z << ",";
              }
            }
          }
          out << std::endl;
          out.close();
        std::cout << "New records append to the handrecord.csv file" << std::endl;
        _stprintf_s(ss, _T("New records append to the handrecord.csv file"));
        printCommand();
      }else{
        std::cout << "Failed to open the handrecord.csv, the command 'm' while not work" << std::endl;
        _stprintf_s(ss, _T("Failed to open the handrecord.csv, the command 'm' while not work"));
        printCommand();
      }
        }
        rId++;
      reading = false;
      if(argc > 2){
          char cmd[1024];
          sprintf(cmd, "%s %s", argv[1], argv[2]);
          system(cmd);
      }else{
          system("echo no file input");
      }
    }else if(c == 'q'){
      break;
    }
    out.close();
    /*
    else
      break;
    */
  }

  // Remove the sample listener when done
  controller.removeListener(listener);
  closegraph();

  return 0;
}
