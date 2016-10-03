//
//  GameScene.hpp
//  LLK
//
//  Created by 白菜哥 on 16/9/16.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "GameConfig.h"
#include "GameItem.hpp"

USING_NS_CC;

class GameScene : public LayerColor{

public:
    
    static Scene *createScene();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void playBGM_1();
    void playBGM_2();
    void playBGM_3();
    
    void autoClear(Ref *spender);
    
    void initUI();
    void initMap();
    void drawMap();
    
    void refreshMap();
    
    bool link(Vec2 p1,Vec2 p2);
    bool linkD(Vec2 p1,Vec2 p2);
    
    Vec2 indextoScreen(int x, int y);
    Vec2 screentoIndex(float x, float y);
    
    void update(float dt);
    
    //CC_PROPERTY(GameItem *, __selectItem, selectItem);
    
private:
    
    ProgressTimer *pProgress;
    Label *pTimeLabel;
    DrawNode *pDraw;
    int mMap[xCount][yCount] = {0};
    int mMapTest[xCount][yCount] = {0};
    
    bool isPauseBGM;
    bool isMapTest;
    bool isHelp;
    int testCount;
    
    std::vector<cocos2d::Vec2> pSelected;
    std::vector<cocos2d::Vec2> pPath;
    
    Vec2 mPre;
    MenuItemSprite *bgmBtn;
    
    void expandX(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector);
    void expandY(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector);
    
    void drawLine();
    void clearMatched();
    void pauseBGM();
    bool mapTest();
    void changeMap();
    
    void help();
    
    CREATE_FUNC(GameScene);
};

#endif /* GameScene_hpp */
