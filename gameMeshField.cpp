
//main.hを一番上にしましょう。
#include "main.h"
#include "meshField.h"
#include "gameScene.h"
#include "scene.h"
#include "player.h"
#include "transform3DAnimationComponent.h"


float g_FieldHeight[FIELD_X][FIELD_Z] =
{
	{0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,4.0f,5.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,4.0f,7.0f,8.0f,10.0f,9.5f,7.0f,5.0f,5.0f,6.0f,4.0f,5.0f,6.0f,6.0f,6.0f,7.0f,6.0f,6.0f,5.0f,4.0f,6.5f,8.0f,8.0f,6.0f,4.0f,5.0f,5.0f,5.0f,6.0f,7.0f,5.0f,6.0f,6.0f,4.0f,7.0f,8.0f,9.5f,11.0f,13.0f,16.0f,15.0f,14.0f,13.0f,0.0f,0.0f},
	{0.0f,4.0f,2.0f,1.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,4.0f,2.0f,1.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,8.0f,17.0f,11.0f,10.0f,12.0f,0.0f},
	{0.0f,5.0f,5.0f,5.0f,7.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,5.0f,0.0f},
	{0.0f,4.0f,5.0f,6.0f,7.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,6.0f,0.0f},
	{0.0f,4.0f,5.0f,5.0f,4.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,7.0f,0.0f},
	{0.0f,7.0f,2.0f,4.0f,3.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,8.0f,0.0f},
	{0.0f,6.0f,2.0f,4.0f,4.0f,2.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,10.0f,0.0f},
	{0.0f,6.0f,2.0f,3.0f,2.0f,1.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,9.0f,0.0f},
	{0.0f,6.0f,2.0f,0.0f,1.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,6.0f,0.0f},
	{0.0f,5.0f,3.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,5.0f,0.0f},
	{0.0f,5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,7.0f,0.0f},
	{0.0f,6.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,4.0f,0.0f},
	{0.0f,5.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,5.0f,0.0f},
	{0.0f,7.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,5.0f,0.0f},
	{0.0f,7.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,6.0f,0.0f},
	{0.0f,6.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,7.0f,0.0f},
	{0.0f,5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,10.0f,0.0f},
	{0.0f,5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,6.0f,0.0f},
	{0.0f,5.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,1.0f,1.0f,1.0f,6.0f,6.0f,7.0f,5.0f,4.0f,5.0f,5.0f,5.0f,5.0f,7.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,5.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,6.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,6.0f,0.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,7.0f,0.0f},
	{0.0f,5.0f,4.0f,5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,5.0f,6.0f,6.0f,6.0f,7.0f,6.0f,6.0f,5.0f,5.0f,7.0f,0.0f},
	{0.0f,4.0f,2.0f,1.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,8.0f,0.0f},
	{0.0f,5.0f,5.0f,5.0f,7.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,6.0f,0.0f},
	{0.0f,4.0f,5.0f,6.0f,7.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,5.0f,0.0f},
	{0.0f,4.0f,5.0f,5.0f,4.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,4.0f,0.0f},
	{0.0f,7.0f,2.0f,4.0f,3.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,6.0f,0.0f},
	{0.0f,6.0f,2.0f,4.0f,4.0f,2.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,5.0f,0.0f},
	{0.0f,6.0f,2.0f,3.0f,2.0f,1.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,6.0f,0.0f},
	{0.0f,6.0f,2.0f,0.0f,1.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,4.0f,0.0f},
	{0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,7.0f,0.0f},
	{0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,6.0f,0.0f},
	{0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,8.0f,0.0f},
	{0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,9.0f,0.0f},
	{0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,10.0f,0.0f},
	{0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,11.0f,0.0f},
	{0.0f,6.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,9.0f,0.0f},
	{0.0f,5.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,7.0f,0.0f},
	{0.0f,5.0f,5.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,6.0f,0.0f},
	{0.0f,5.0f,4.0f,5.0f,6.0f,6.0f,7.0f,7.0f,5.0f,5.0f,5.0f,4.0f,6.0f,6.0f,7.0f,5.0f,4.0f,5.0f,5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,5.0f,4.0f,5.0f,6.0f,6.0f,7.0f,7.0f,5.0f,5.0f,5.0f,4.0f,6.0f,6.0f,7.0f,5.0f,4.0f,5.0f,5.0f,5.0f,0.0f,5.0f,0.0f},
	{0.0f,9.0f,5.0f,7.0f,6.0f,7.0f,9.0f,8.0f,9.0f,11.0f,12.0f,11.0f,9.0f,8.0f,5.0f,7.0f,7.0f,7.0f,7.0f,7.0f,8.0f,9.0f,13.0f,18.0f,17.0f,22.0f,29.0f,32.0f,33.0f,30.0f,27.0f,24.0f,21.0f,19.0f,18.0f,14.0f,12.0f,10.0f,7.0f,6.0f,8.0f,9.0f,9.0f,10.0f,8.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,2.0f,2.0f,3.0f,4.0f,6.0f,8.0f,7.0f,5.0f,5.0f,4.0f,3.0f,4.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,2.0f,2.0f,3.0f,4.0f,6.0f,8.0f,7.0f,5.0f,5.0f,4.0f,3.0f,4.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f}
	

};


void MeshField::Init()
{
	//頂点バッファ生成
	{
		for (int x = 0; x < FIELD_X; x++)//
		{
			for (int z = 0; z < FIELD_Z; z++)//
			{
				m_Vertex[x][z].Position = XMFLOAT3((x - 10) * 5.0f, g_FieldHeight[x][z], (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x, z);
			}
		}

		//法線ベクトル計算
		for (int x = 1; x < (FIELD_X - 1); x++)//
		{
			for (int z = 1; z < (FIELD_Z - 1); z++)//
			{
				XMFLOAT3 vx, vz, vn;
				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

				//外積
				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				//正規化(長さ１にする)
				float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn.x /= len;
				vn.y /= len;
				vn.z /= len;

				m_Vertex[x][z].Normal = vn;

			}

		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * (FIELD_X) * (FIELD_Z);//
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ
	{
		unsigned int index[((FIELD_X + 1) * 2) * (FIELD_Z - 1) - 2];//

		int i = 0;
		for (int x = 0; x < FIELD_X - 1; x++)//
		{
			for (int z = 0; z < FIELD_Z; z++)//
			{
				index[i] = x * FIELD_X + z;//
				i++;

				index[i] = (x + 1) * (FIELD_X) + z;//
				i++;
			}
			if (x == (FIELD_X - 2))//
				break;
			index[i] = (x + 1) * (FIELD_X) + (FIELD_Z - 1);//
			i++;

			index[i] = (x + 1) * (FIELD_X);//
			i++;
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((FIELD_X + 1) * 2) * (FIELD_X - 1) - 2;//
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\jimen.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

}

void MeshField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void MeshField::Update()
{

}

void MeshField::Draw()
{


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Renderer::SetDepthEnable(false);

	//ポリゴン設定
	//Renderer::GetDeviceContext()->Draw(21 * 21, 0);
	Renderer::GetDeviceContext()->DrawIndexed(((FIELD_X + 1) * 2) * (FIELD_Z - 1) - 2, 0, 0);

	//Renderer::SetDepthEnable(true);
	/*DrawImGui();*/

}

void MeshField::DrawImGui()
{

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();

	int x, z;
	//ブロック番号算出
	x = playerPos.x / 5.0f + 10.0f;		//Initで設定してる値
	z = playerPos.z / -5.0f + 10.0f;

	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x =  playerPos.x - pos1.x;
	v1p.y =  playerPos.y - pos1.y;
	v1p.z =  playerPos.z - pos1.z;

	//外積
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;
	XMFLOAT3 n;
	if (cy > 0.0f)
	{
		//左上ポリゴン
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		//外積
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		//右上ポリゴン
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		//外積
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	////高さ取得
	//py = -((playerPos.x - pos1.x) * n.x + (playerPos.z - pos1.z) * n.z) / n.y + pos1.y;

	//{

	//	ImGui::Begin("MeshField");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	//	ImGui::Text("BlockNumber%d,%d", x, z);
	//	ImGui::Text("Gaiseki = %.1f", cy);
	//	ImGui::Text("Hight = %.1f", py);


	//	ImGui::End();

	//}

}

float MeshField::GetHeight(XMFLOAT3 pos)
{
	int x, z;
	//ブロック番号算出
	x = pos.x / 5.0f + 10.0f;		//Initで設定してる値
	z = pos.z / -5.0f + 10.0f;

	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x = pos.x - pos1.x;
	v1p.y = pos.y - pos1.y;
	v1p.z = pos.z - pos1.z;

	//外積
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;
	XMFLOAT3 n;
	if (cy > 0.0f)
	{
		//左上ポリゴン
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		//外積
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		//右上ポリゴン
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		//外積
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	//高さ取得
	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}
