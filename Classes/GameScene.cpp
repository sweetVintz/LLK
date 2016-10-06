//
//  GameScene.cpp
//  LLK
//
//  Created by 白菜哥 on 16/9/16.
//
//

#include "GameScene.hpp"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include "GameConfig.h"
#include "GameItem.hpp"
#include <math.h>

using namespace CocosDenshion;

std::vector<Vec2> selectedExpand;
std::vector<Vec2> selectExpand;

Scene *GameScene::createScene(){

    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init(){

    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
        
        return false;
    }
    
    initUI();
    playBGM_1();
    initMap();
    
    return true;
}
#pragma mark 背景音乐_1
void GameScene::playBGM_1(){

    if (!isPauseBGM) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_Back2new ,false);
        scheduleOnce(SEL_SCHEDULE(&GameScene::playBGM_2), 60);
    }
}
#pragma mark 背景音乐_2
void GameScene::playBGM_2(){
    
    if (!isPauseBGM) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_Back3new, false);
        scheduleOnce(SEL_SCHEDULE(&GameScene::playBGM_3), 62);
    }
}
#pragma mark 背景音乐_3
void GameScene::playBGM_3(){
    
    if (!isPauseBGM) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_mainmenu, false);
        scheduleOnce(SEL_SCHEDULE(&GameScene::playBGM_1), 65);
    }
}

#pragma mark 初始化UI
void GameScene:: initUI(){
    
    isPauseBGM = false;
    isMapTest = false;
    isHelp = false;
    testCount = 0;

    auto Tex = Director::getInstance()->getTextureCache();
    auto background = Sprite::createWithTexture(Tex->getTextureForKey(s_backgound));
    background->setPosition(screenSize.width/2,screenSize.height/2);
    addChild(background);
    
    auto cloud_1 = Sprite::createWithTexture(Tex->getTextureForKey(s_backgound_cloud1));
    cloud_1->setAnchorPoint(Vec2(0, 0));
    cloud_1->setPosition(0,screenSize.height-cloud_1->getContentSize().height+36);
    addChild(cloud_1);
    
    auto cloudShadow = Sprite::createWithTexture(Tex->getTextureForKey(s_backgound_cloud1));
    cloudShadow->setAnchorPoint(Vec2(0, 0));
    cloudShadow->setFlippedY(true);
    cloudShadow->setOpacity(40);
    cloudShadow->setPosition(cloud_1->getPositionX(),cloud_1->getPositionY()-cloudShadow->getContentSize().height+36);
    addChild(cloudShadow);
    
    auto cloud_2 = Sprite::createWithTexture(Tex->getTextureForKey(s_backgound_cloud2));
    cloud_2->setAnchorPoint(Vec2(0, 0));
    cloud_2->setPosition(cloud_1->getContentSize().width/2,screenSize.height-cloud_2->getContentSize().height+36);
    addChild(cloud_2);
    
    auto island = Sprite::createWithTexture(Tex->getTextureForKey(s_island));
    island->setAnchorPoint(Vec2(1, 0));
    island->setPosition(screenSize.width-40,screenSize.height-island->getContentSize().height-47*2);
    addChild(island);
    
    auto islandShadow = Sprite::createWithTexture(Tex->getTextureForKey(s_island));
    islandShadow->setAnchorPoint(Vec2(1, 0));
    islandShadow->setOpacity(40);
    islandShadow->setPosition(island->getPositionX(),island->getPositionY()-islandShadow->getContentSize().height+15);
    islandShadow->setFlippedY(true);
    addChild(islandShadow);
    
    // 时间条背景框
    auto progressFrame = Sprite::createWithTexture(Tex->getTextureForKey(s_time_slot));
    // 锚点，左下角
    progressFrame->setAnchorPoint(Vec2(0, 0));
    
    progressFrame->setPosition(120, screenSize.height-50);
    
    addChild(progressFrame);
    
    auto progressSP = Sprite::createWithTexture(Tex->getTextureForKey(s_time_bars));
    pProgress = ProgressTimer::create(progressSP);
    pProgress->setAnchorPoint(Vec2(0, 0));
    pProgress->setType(ProgressTimer::Type::BAR);
    pProgress->setPosition(progressFrame->getPosition());
    pProgress->setMidpoint(Vec2(0, 0));
    pProgress->setBarChangeRate(Vec2(1, 0));
    pProgress->setPercentage(100);
    addChild(pProgress);
    
    pTimeLabel = Label::createWithSystemFont("100", "Thonburi", 24 );
    pTimeLabel->setAnchorPoint(Vec2(0, 0));
    pTimeLabel->setPosition(400,pProgress->getPositionY());
    pTimeLabel->setColor(Color3B::BLACK);
    addChild(pTimeLabel);
    
    auto tip = Sprite::createWithTexture(Tex->getTextureForKey(s_game_leisure));
    auto menuItemSp = MenuItemSprite::create(tip, tip,CC_CALLBACK_1(GameScene::autoClear, this));
    menuItemSp->setPosition(screenSize.width-100,screenSize.height-100);

    auto bgmOn = Sprite::createWithTexture(Tex->getTextureForKey(s_game_bgmOn));
    auto bgmOff = Sprite::createWithTexture(Tex->getTextureForKey(s_game_bgmOff));
    bgmBtn = MenuItemSprite::create(bgmOn, bgmOff, CC_CALLBACK_0(GameScene::pauseBGM, this));
    bgmBtn->setPosition(menuItemSp->getPositionX(),menuItemSp->getPositionY()-100);
    
    auto help = Sprite::createWithTexture(Tex->getTextureForKey(s_game_help));
    auto helpBtn = MenuItemSprite::create(help, help, CC_CALLBACK_0(GameScene::help, this));
    helpBtn->setPosition(bgmBtn->getPositionX(),bgmBtn->getPositionY()-100);
    helpBtn->setScale(0.25);

    auto menu = Menu::create(menuItemSp, bgmBtn, helpBtn,NULL);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    pDraw = DrawNode::create();
    addChild(pDraw , 100);
    
    
//    // 绘制棋盘上精灵
//    for (int x = 0; x < xCount; x++) {
//        for (int y = 0; y < yCount; y++) {
//            if (mMap[x][y] > 0) {
//                
//                int index = mMap[x][y]-1;
//                
//                log("切割的图片位置是：%d_%d_%d_%f",index,index%6,(int)index/6,iconSize);
//                auto spFrame = SpriteFrame::create(s_game_icon, Rect(index%6*iconSize, (int)index/6*iconSize, iconSize, iconSize));
//                auto spFrame1 = SpriteFrame::create(s_game_icon, Rect(0, 0, iconSize, iconSize));
//                auto sp = GameItem::createItem(spFrame);
//                auto position = indextoScreen(x, y);
//                sp->setPosition(position);
//                int tag = (yCount - 2) * ( x - 1 ) + y;
//                addChild(sp,100,tag);
//            }
//        }
//    }

    
    schedule(SEL_SCHEDULE(&GameScene::update), 1.0);
}

void GameScene::pauseBGM(){

    auto Tex = Director::getInstance()->getTextureCache();
    if (isPauseBGM) {
        isPauseBGM = false;
        auto img = Sprite::createWithTexture(Tex->getTextureForKey(s_game_bgmOn));
        bgmBtn->setNormalImage(img);
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }else{
        isPauseBGM = true;
        auto img = Sprite::createWithTexture(Tex->getTextureForKey(s_game_bgmOff));
        bgmBtn->setNormalImage(img);
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}
#pragma mark 初始化地图
void GameScene::initMap(){
    
    // 这里x用来控制地图数组的每格的值，y用来控制两次反转，即每两个格标记同一个值
    int x = 1;
    int y = 0;
    // 数组下标从0开始，这里从1开始遍历，那么最外面一圈就是0不变
    for (int i = 1; i < xCount - 1; i++) {
        for (int j = 1; j < yCount - 1; j++) {
            // 地图数组赋值
            mMap[i][j] = x;
            mMapTest[i][j]=x;
            // y控制反转，x控制每格值增加，增大到图片总数后从再1开始递增
            if (y == 1) {
                x++;
                y = 0;
                if ( x == iconCount) {
                    x = 1;
                }
            }else{
                y = 1;
            }
        }
    }
    refreshMap();
}

#pragma mark 绘制地图
void GameScene::drawMap(){
    
    // 绘制棋盘上精灵
    for (int x = 0; x < xCount; x++) {
        for (int y = 0; y < yCount; y++) {
            
            mMap[x][y]=mMapTest[x][y];
            
            if (mMap[x][y] > 0) {
                
                char iconName[64] = {0};
                // 格式化图片名
                sprintf(iconName, "%d.png", mMap[x][y]);
                auto position = indextoScreen(x, y);
                // 所有图片已经加到帧缓存，这里直接用名字取一帧，创建一个精灵
                auto icon = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iconName));
                //                icon->setAnchorPoint(Vec2(0, 0));
                icon->setPosition(position);
                // 设置一个tag，方便后续识别点击的是那个图标
                int tag = (yCount - 2) * ( x - 1 ) + y;
                // 这是Z序和tag值，Z序设置大一些确保图标在其他精灵的前面
                addChild(icon, 100, tag);
            }
        }
    }
}
#pragma mark 检测地图是否能够全部消除
bool GameScene::mapTest(){
    
    // 如果能够消除到仅剩最后两对便返回true
    for (int i = 0; i < (xCount-2)*(yCount-2)/2; i++) {
        for (int y = 1; y < yCount; y++) {
            for (int x = 1; x < xCount; x++) {
                if (mMap[x][y] != 0) {
                    for (int j = y; j < yCount - 1; j++) {
                        if (j == y) {
                            for (int i = x + 1; i < xCount - 1; i++) {
                                if (mMap[i][j] == mMap[x][y] && link(Vec2(x, y), Vec2(i, j))) {
                                    testCount++;
                                    mMap[i][j] = 0;
                                    mMap[x][y] = 0;
                                }
                            }
                        }else{
                            for (int i = 1; i < xCount - 1; i++) {
                                if (mMap[i][j] == mMap[x][y] && link(Vec2(x, y), Vec2(i, j))) {
                                    testCount++;
                                    mMap[i][j] = 0;
                                    mMap[x][y] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (testCount>=(xCount-2)*(yCount-2)/2) {
        return true;
    }else{
        return false;
    }

}

#pragma mark 刷新地图（打乱地图）
void GameScene::refreshMap(){

    testCount = 0;
    srand((unsigned int)time(NULL));
    
    int tempX,tempY,tempM;
    for (int x=1; x<xCount-1; x++) {
        for (int y=1; y<yCount-1; y++) {
            
            tempX = 1+rand()%(xCount-2);
            tempY = 1+rand()%(yCount-2);
            tempM = mMap[x][y];
            mMap[x][y] = mMap[tempX][tempY];
            mMap[tempX][tempY] = tempM;
        }
    }

    for (int x=1; x<xCount-1; x++) {
        for (int y=1; y<yCount-1; y++) {
            
            mMapTest[x][y] = mMap[x][y];
        }
    }

    if (mapTest()) {
        drawMap();
    }else{
        initMap();
    }
}

#pragma mark 游戏帮助（提示画线）
void GameScene::help(){

    isHelp = true;
    for (int y = 1; y < yCount; y++)
        for (int x = 1; x<xCount; x++) {
            if (mMap[x][y] != 0) {
                for (int j = y; j<yCount-1; j++) {
                    if (j == y) {
                        for (int i = x; i<xCount-1; i++) {
                            if (mMap[x][y]==mMap[i][j] && link(Vec2(x, y), Vec2(i, j))) {
                                drawLine();
                                return;
                            }
                        }
                    }else{
                        for (int i = 1; i<xCount-1; i++) {
                            if (mMap[x][y]==mMap[i][j] && link(Vec2(x, y), Vec2(i, j))) {
                                drawLine();
                                return;
                            }
                        }
                    }
                }
            }
        }
    
}

#pragma mark 数组坐标转换为屏幕坐标
Vec2 GameScene::indextoScreen(int x, int y)
{
    return Vec2(x * iconSize+15, y * iconSize+15);
}
#pragma mark 屏幕坐标转换为地图坐标
Vec2 GameScene::screentoIndex(float x, float y)
{
    int ix = ceil((x - iconSize / 2) / iconSize);
    
    int iy = ceil((y - iconSize / 2) / iconSize);
    
    log("转换后的 iX 是：%d   转换后的 iY 是：%d",ix,iy);
    
    if (ix < xCount && iy < yCount) {
        return Vec2(ix ,iy);
    }else{
        return Vec2::ZERO;
    }
}
#pragma mark 实时更新计时器
void GameScene::update(float dt){

    int current = pProgress->getPercentage();
    if (current == 0) {
        return;
    }
    else{
        current--;
        pProgress->setPercentage(current);
        char time[] = {25};
        sprintf(time, "%d",current);
        pTimeLabel->setString(time);
    }
}
#pragma mark 连接判断 － 1
bool GameScene::link(cocos2d::Vec2 selected, cocos2d::Vec2 select){
    
    //点击的是同一个点，返回false
    if (selected.equals(select)) {
        return false;
    }
    // 路径点集，是一个盛放Vec2对象的vector
    pPath.clear();
    
    // 判断点击的两个点是不是图案一致
    if (mMap[(int)selected.x][(int)selected.y] == mMap[(int)select.x][(int)select.y]) {
        // 直连
        if (linkD(selected, select)) {
            pPath.push_back(selected);
            pPath.push_back(select);
            return true;
        }
        
        // 一个拐角，对角可直连
        auto p = Vec2(selected.x, select.y);
        if (mMap[(int)p.x][(int)p.y] == 0) {
            if (linkD(selected, p) && linkD(p, select)) {
                pPath.push_back(selected);
                pPath.push_back(p);
                pPath.push_back(select);
                return true;
            }
        }
        
        // 一个拐角，对角可直连
        p = Vec2(select.x, selected.y);
        if (mMap[(int)p.x][(int)p.y] == 0) {
            if (linkD(selected, p) && linkD(p, select)) {
                pPath.push_back(selected);
                pPath.push_back(p);
                pPath.push_back(select);
                return true;
            }
        }
        
        // X扩展，判断是否有可直连的点
        expandX(selected, selectedExpand);
        expandX(select, selectExpand);
        
        for (auto pt1 : selectedExpand) {
            for (auto pt2 : selectExpand) {
                if (pt1.x == pt2.x) {
                    if (linkD(pt1, pt2)) {
                        pPath.push_back(selected);
                        pPath.push_back(pt1);
                        pPath.push_back(pt2);
                        pPath.push_back(select);
                        return true;
                    }
                }
            }
        }
        
        // Y扩展，判断是否有可直连的点
        expandY(selected, selectedExpand);
        expandY(select, selectExpand);
        for (auto pt1 : selectedExpand) {
            for (auto pt2 : selectExpand) {
                if (pt1.y == pt2.y) {
                    if (linkD(pt1, pt2)) {
                        pPath.push_back(selected);
                        pPath.push_back(pt1);
                        pPath.push_back(pt2);
                        pPath.push_back(select);
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    return false;
}

#pragma mark 连接判断 － 2
bool GameScene::linkD(cocos2d::Vec2 p1, cocos2d::Vec2 p2){

    // X坐标一致，逐个扫描Y坐标，中间如果都是通路，那么可以直连
    if (p1.x == p2.x) {
        int y1 = MIN(p1.y, p2.y);//取两个数中较小的一个
        int y2 = MAX(p1.y, p2.y);//取两个数中较大的一个
        bool flag = true;
        for (int y = y1 + 1; y < y2; y++) {
            if (mMap[(int)p1.x][y] != 0) {
                flag = false;
                break;
            }
        }
        
        if (flag) {
            return true;
        }
    }
    
    // Y坐标一致，逐个扫描X坐标，中间如果都是通路，那么可以直连
    if (p1.y == p2.y) {
        int x1 = MIN(p1.x, p2.x);
        int x2 = MAX(p1.x, p2.x);
        bool flag = true;
        for (int x = x1 + 1; x < x2; x++) {
            if (mMap[x][(int)p1.y] != 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return true;
        }
    }
    
    return false;
}

#pragma mark 画线

void GameScene::drawLine(){
    
    if (isHelp) {
        // 画线
        if (pPath.size() >= 2) {
            Vec2 *vecs = new Vec2[pPath.size()];
            for (int i = 0 ; i < pPath.size(); i++) {
                vecs[i] = indextoScreen(pPath.at(i).x, pPath.at(i).y);
            }
            
            // 随机给线条一个颜色
            auto color = CCRANDOM_0_1();
            
            // 循环画线段，只有线段可以设置线条宽度
            for (int i = 0 ; i < pPath.size(); i++) {
                if ( i > 0) {
                    pDraw->drawSegment(vecs[i-1], vecs[i], 5, Color4F(color, color, color, 1));
                }
            }
            // 清除连通的图标，同时清除路径点
            scheduleOnce(SEL_SCHEDULE(&GameScene::clearMatched), 0.5);
        }
    }else{
        // 画线
        if (pPath.size() >= 2) {
            Vec2 *vecs = new Vec2[pPath.size()];
            for (int i = 0 ; i < pPath.size(); i++) {
                vecs[i] = indextoScreen(pPath.at(i).x, pPath.at(i).y);
            }
            
            // 随机给线条一个颜色
            auto color = CCRANDOM_0_1();
            
            // 循环画线段，只有线段可以设置线条宽度
            for (int i = 0 ; i < pPath.size(); i++) {
                if ( i > 0) {
                    pDraw->drawSegment(vecs[i-1], vecs[i], 5, Color4F(color, color, color, 1));
                }
            }
            
            // 连通的两个图标对应的地图数组置0
            Vec2 p1 = pPath.front();
            mMap[(int)p1.x][(int)p1.y] = 0;
            Vec2 p2 = pPath.back();
            mMap[(int)p2.x][(int)p2.y] = 0;
            
            // 删掉线段的顶点数组
            delete []vecs;
            
            // 清除连通的图标，同时清除路径点
            
            scheduleOnce(SEL_SCHEDULE(&GameScene::clearMatched), 0.2);
        }
    }
}

#pragma mark 消除后清理数据

void GameScene::clearMatched(){
    
    if (isHelp) {
        
        pDraw->clear();
        pSelected.clear();
        pPath.clear();
        isHelp = false;
        
    }else{
        
        pDraw->clear();
        
        Vec2 p1 = pPath.front();
        Vec2 p2 = pPath.back();
        
        auto x1 = (int)p1.x;
        auto y1 = (int)p1.y;
        auto tag1 = (yCount - 2) * ( x1 - 1 ) + y1;
        removeChildByTag(tag1);
        
        auto x2 = (int)p2.x;
        auto y2 = (int)p2.y;
        auto tag2 = (yCount - 2) * ( x2 - 1 ) + y2;
        removeChildByTag(tag2);
        
        pSelected.clear();
        pPath.clear();
    
    }
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){

    return true;
}
void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){

}
void GameScene:: onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event){

}
void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){

    float x = touch->getLocation().x;
    float y = touch->getLocation().y;
    auto point = screentoIndex(x, y);
    log("点击的 X 是：%f   点击的 Y 是：%f",x,y);
    
    if (mMap[(int)point.x][(int)point.y] > 0) {
        //已经有一个选中的对象，判断当前选中的跟之前的是否匹配
        if (pSelected.size() == 1) {
            if (link(pSelected.front(), point)) {
                pSelected.push_back(point);
                drawLine();
            }else{
                mPre = (Vec2)pSelected.front();
                pSelected.clear();
                pSelected.push_back(point);
            }
        }else{
            pSelected.push_back(point);
        }
    }
    // 前一次点击的点
    if (!mPre.equals(Vec2::ZERO)) {
        int x = (int)mPre.x;
        int y = (int)mPre.y;
        int tag =  (yCount - 2) * ( x - 1 ) + y;
        auto selectedIcon = getChildByTag(tag);
        
        if (selectedIcon != NULL) {
            // 恢复原大小
            selectedIcon->setScale(1.0);
            // 恢复原Z序
            selectedIcon->setLocalZOrder(100);
        }
    }
    
    
    // 绘制选择图标，选中时变大
    for ( Vec2 position: pSelected ) {
        int x = (int)position.x;
        
        int y = (int)position.y;
        
        int tag = (yCount - 2) * ( x - 1 ) + y;
        
        auto slectedIcon = getChildByTag(tag);
        
        // 放大1.2倍
        slectedIcon->setScale(1.2);
        
        // Z序提前，放在所有精灵前面
        slectedIcon->setLocalZOrder(101);
    }
}

#pragma mark X方向的寻路
void GameScene::expandX(Vec2 v, std::vector<Vec2> &vector)
{
    // 注意第二个参数vector是传的引用
    vector.clear();
    
    // X轴扩展到边界，如果都是空的就将扩展的点放入容器
    for (int x = (int)v.x + 1; x < xCount; x++) {
        if (mMap[x][(int)v.y] != 0) {
            break;
        }
        vector.push_back(Vec2(x, (int)v.y));
    }
    
    // X轴扩展到边界，如果都是空的就将扩展的点放入容器
    for (int x = (int)v.x - 1; x >= 0; x--) {
        if (mMap[x][(int)v.y] != 0) {
            break;
        }
        vector.push_back(Vec2(x, (int)v.y));
    }
}

#pragma mark Y方向的寻路
void GameScene::expandY(Vec2 v, std::vector<Vec2> &vector)
{
    vector.clear();
    
    for (int y = (int)v.y + 1; y < yCount; y++) {
        if (mMap[(int)v.x][y] != 0) {
            break;
        }
        vector.push_back(Vec2((int)v.x, y));
    }
    
    for (int y = (int)v.y - 1; y >= 0 ; y--) {
        if (mMap[(int)v.x][y] != 0) {
            break;
        }
        vector.push_back(Vec2((int)v.x, y));
    }
}

void GameScene::autoClear(Ref *spender){

    auto rote = RotateBy::create(0.05,20);
    auto seq = Sequence::create(rote,rote->reverse(),rote->reverse(),rote->clone(), NULL);
    ((Sprite *)spender)->runAction(seq);
}