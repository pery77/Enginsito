#include "engine.h"
class Game : public Engine{

public:
    virtual void Init() override;
    virtual void Tick() override;		 
	virtual void Draw() override;
	virtual void OverDraw() override;

    virtual ~Game(){};
};