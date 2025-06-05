#pragma once


class Input
{
private:
    BYTE m_OldKeyState[256];
	BYTE m_KeyState[256];

	Input() = default;
	~Input() = default;

public:

	static Input* GetInstance() {

		static Input* input = new Input;
		return input;
	}


     void Init();
     void Uninit();
     void Update();
    
     bool GetKeyPress( BYTE KeyCode );
     bool GetKeyTrigger( BYTE KeyCode );

};
