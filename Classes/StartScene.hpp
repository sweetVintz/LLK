//
//  StartScene.hpp
//  LLK
//
//  Created by 白菜哥 on 16/9/16.
//
//

#ifndef StartScene_hpp
#define StartScene_hpp

#include <stdio.h>

USING_NS_CC;

class StartScene : public Layer{

public:
    
    static Scene *createScene();
    
    virtual bool init();
    void update(float dt);
    
    void initUI();
    void gameStart();
    
    Sprite *pBird;
    
    CREATE_FUNC(StartScene);
};

#endif /* StartScene_hpp */
