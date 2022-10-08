#include "DxLib.h"
#include "Rule.h"
#include "UI.h"
#include "BackGround.h"
#include "Door.h" 
#include "Player.h"
#include "Tool.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //フルスクリーンモード
    ChangeWindowMode(FALSE);
    //解像度・カラービット数の指定
    SetGraphMode(1920, 1080, 32);

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        // エラーが起きたら直ちに終了 
        return -1;
    }

    Rule* rule = new Rule();
    UI* ui = new UI();
    BackGround* background = new BackGround();
    Door* door = new Door();
    Player* player = new Player();
    Tool* tool = new Tool();
    int End;

    rule->SetStartTime();
    while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        End = rule->CheckEnd(player->GetDeadNum());
        //プレイヤー再生成
        if (player->GetDead())
        {
            delete player;
            player = new Player();
        }
        if (player->GetGoal())
        {
            delete player;
            player = new Player();
            rule->AddScore();
        }
        

        ClearDrawScreen();
        rule->SetNowTime();
        rule->SetDeltaTime();

        //更新処理郡
        door->Update(rule->GetDeltaTime());
        player->Update(rule->GetDeltaTime(),door,tool);
        tool->Update(rule->GetDeltaTime());

        //描画処理郡
        background->Draw();
        door->Draw();
        player->Draw();
        tool->Draw();

        //UI処理
        ui->WriteLimitTime(rule->GetLimitTime());
        ui->WriteScore(rule->GetScore());
        ui->DrawPlayerDead(player->GetDeadNum());
       
        ScreenFlip();
        rule->SetPrevTime();
    }

    delete rule;
    delete ui;
    delete background;
    delete player;
    delete tool;

    DxLib_End();				// ＤＸライブラリ使用の終了処理 
    return 0;				// ソフトの終了  

}