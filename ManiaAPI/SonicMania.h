#pragma once
#define _USE_MATH_DEFINES
#include <MemAccess.h>
#include <cmath>
#include <cstdio>


#pragma region PreprocessorDefines

#define BitFlag(type, enumType) inline type operator|(type a, type b) { return (type)((enumType)a | (enumType)b); }
#define ALIGN(x) __declspec(align(x))

#pragma endregion

typedef unsigned char byte;
typedef unsigned short ushort;

namespace SonicMania
{
#pragma region TypesDefs

    typedef void(__cdecl* Ability)();
    typedef void(__cdecl* PlayerStatus)();
    typedef void(__cdecl* InputStatus)();

#pragma endregion

#pragma region Declares
    // Entities
    struct Entity;
    struct EntityBase;
    struct EntityPlayer;
    struct EntityCamera;
    struct EntityItemBox;
    struct EntityRing;
    struct EntityUIText;

    //Objects
    struct Object;
    struct Obj_Player;
    struct Obj_Camera;
    struct Obj_Ring;
    struct Obj_Platform;
    struct Obj_SpecialRing;
    struct Obj_ItemBox;
    struct Obj_Spring;
    struct Obj_Animals;
    struct Obj_ScoreBonus;
    struct Obj_Explosion;
    struct Obj_Spikes;
    struct Obj_Door;
    struct Obj_Button;
    struct Obj_StarPost;
    struct Obj_BreakableWall;
    struct Obj_LRZBreakableWall;
    struct Obj_CollapsingPlatform;
    struct Obj_ForceSpin;
    struct Obj_PlaneSwitch;
    struct Obj_GHZSetup;
    struct Obj_LRZ1Setup;
    struct Obj_ZONE;
    struct Obj_Clapperboard;
    struct Obj_Lovetester;
    struct Obj_PauseMenu;
    struct Obj_FXRuby;
    struct Obj_DebugMode;
    struct GameOptions;

    //Misc
    struct Hitbox;

#pragma endregion



#pragma region Enums
    // Enum
    enum Scene : short
    {
        Scene_NONE = -1,
        // Presentation 
        Scene_Logo,
        Scene_Title,
        Scene_MainMenu,
        Scene_ThanksForPlaying,
        Scene_LevelSelect,
        Scene_Credits,
        Scene_Continue,
        Scene_Summary,
        Scene_Summary_e,
        // Mania Mode
        Scene_GHZ1,
        Scene_GHZ2,
        Scene_CPZ1,
        Scene_CPZ2,
        Scene_SPZ1,
        Scene_SPZ2,
        Scene_FBZ1,
        Scene_FBZ2,
        Scene_PSZ1,
        Scene_PSZ2,
        Scene_SSZ1,
        Scene_SSZ2,
        Scene_SSZ2M,
        Scene_HCZ1,
        Scene_HCZ2,
        Scene_MSZ1,
        Scene_MSZ1K,
        Scene_MSZ2,
        Scene_OOZ1,
        Scene_OOZ2,
        Scene_LRZ1,
        Scene_LRZ2,
        Scene_LRZ3,
        Scene_MMZ1,
        Scene_MMZ2,
        Scene_TMZ1,
        Scene_TMZ2,
        Scene_TMZ3,
        Scene_ERZ,
        // Encore Mode
        Scene_GHZ1_e,
        Scene_GHZ2_e,
        Scene_CPZ1_e,
        Scene_CPZ2_e,
        Scene_SPZ1_e,
        Scene_SPZ2_e,
        Scene_FBZ1_e,
        Scene_FBZ2_e,
        Scene_PSZ1_e,
        Scene_PSZ2_e,
        Scene_SSZ1_e,
        Scene_SSZ2_e,
        Scene_SSZ2M_e,
        Scene_HCZ1_e,
        Scene_HCZ2_e,
        Scene_MSZ1_e,
        Scene_MSZ2_e,
        Scene_OOZ1_e,
        Scene_OOZ2_e,
        Scene_LRZ1_e,
        Scene_LRZ2_e,
        Scene_LRZ3_e,
        Scene_MMZ1_e,
        Scene_MMZ2_e,
        Scene_TMZ1_e,
        Scene_TMZ2_e,
        Scene_TMZ3_e,
        // Special Stage
        Scene_SpecialStage1,
        Scene_SpecialStage2,
        Scene_SpecialStage3,
        Scene_SpecialStage4,
        Scene_SpecialStage5,
        Scene_SpecialStage6,
        Scene_SpecialStage7,
        Scene_SpecialStage1_e,
        Scene_SpecialStage2_e,
        Scene_SpecialStage3_e,
        Scene_SpecialStage4_e,
        Scene_SpecialStage5_e,
        Scene_SpecialStage6_e,
        Scene_SpecialStage7_e,
        // Blue Sphere (+0 - +31)
        Scene_BlueSphere,
        Scene_BlueSphere_Random = Scene_BlueSphere + 32,
        Scene_BlueSphere_Random2,
        // Pinball
        Scene_Pinball,
        // Extras
        Scene_PuyoPuyo,
        Scene_DAGarden,
        Scene_DAGarden_e,
        // Cutscene
        Scene_Cutscene_AngelIsland,
        Scene_Cutscene_AngelIsland_e,
        Scene_Cutscene_GreenHillZone,
        Scene_Cutscene_GreenHillZone2,
        Scene_Cutscene_MirageSaloonKnucklesIntro,
        Scene_Cutscene_StardustSpeedwayTimeTravel,
        Scene_TryAgain,
        Scene_Cutscene_MirageSaloonEnd_e,
        Scene_TryAgain_e,
        // Video
        Scene_BadEnd_Sonic,
        Scene_BadEnd_Sonic2,
        Scene_BadEnd_Tails,
        Scene_BadEnd_Knux,
        Scene_BadEnd_Mighty,
        Scene_BadEnd_Ray,
        Scene_GoodEnd,
        Scene_TrueEnd
    };

    enum Category : byte
    {
        Presentation,
        ManiaMode,
        EncoreMode,
        SpecialStage,
        BlueSpheres,
        Pinball,
        Extras,
        Cutscenes,
        Videos
    };

    enum ShieldType
    {
        ShieldType_None,
        ShieldType_Normal,
        ShieldType_Water,
        ShieldType_Flame,
        ShieldType_Lightning
    };

    enum Character : int
    {
        Character_None = 0b00000,
        Character_Sonic = 0b00001, // 1 << 0 ( 1)
        Character_Tails = 0b00010, // 1 << 1 ( 2)
        Character_Knux = 0b00100, // 1 << 2 ( 4)
        Character_Mighty = 0b01000, // 1 << 3 ( 8)
        Character_Ray = 0b10000  // 1 << 4 (16)
    };

    enum GameMode : int
    {
        GameMode_ManiaMode,
        GameMode_EncoreMode,
        GameMode_TimeAttack,
        GameMode_Competition
    };

    enum Filter : byte
    {
        Filter_None = 0b0000, // 0
        Filter_ManiaMode = 0b0001, // 1
        Filter_Unknown = 0b0010, // 2
        Filter_EncoreMode = 0b0100  // 4
    };
    BitFlag(Filter, byte)

        enum GameStates : byte
    {
        GameState_NotRunning = 0b0000, // 0
        GameState_Running = 0b0001, // 1
        GameState_SoftPause = 0b0010, // 2
        GameState_HardPause = 0b0100, // 4
        GameState_DevMenu = 0b1000  // 8
    };
    BitFlag(GameStates, byte)

        enum DrawingFX : BYTE
    {
        DrawingFX_None = 0,
        DrawingFX_Flip = 1,
        DrawingFX_Rotate_NoFlip = 2,
        DrawingFX_Rotate = 3,
        DrawingFX_Scale_NoFlip = 4,
        DrawingFX_Scale = 5,
        DrawingFX_RotoZoom_NoFlip = 6,
        DrawingFX_RotoZoom = 7,
    };
    BitFlag(DrawingFX, byte)

        enum InkEffect : byte
    {
        Ink_None,
        Ink_Blend,
        Ink_Alpha,
        Ink_Add,
        Ink_Subtract,
        Ink_Distort,
        Ink_Masked,
        Ink_Unmasked
    };

    enum ActiveStates : byte
    {
        ACTIVE_NEVER,
        ACTIVE_ALWAYS,
        ACTIVE_NORMAL,
        ACTIVE_PAUSED,
        ACTIVE_BOUNDS,
        ACTIVE_XBOUNDS,
        ACTIVE_YBOUNDS,
        ACTIVE_BOUNDS2,
        ACTIVE_NEVER3 = 255,
    };

    enum FlipFlags : byte
    {
        FLIP_NONE, // 0b00
        FLIP_X,    // 0b01
        FLIP_Y,    // 0b10
        FLIP_XY,   // 0b11
    };

    enum Scope : byte
    {
        Scope_None,
        Scope_Global,
        Scope_Stage
    };

    enum ItemBoxItem : byte
    {
        ItemBoxItem_Ring,
        ItemBoxItem_ShieldNormal,
        ItemBoxItem_ShieldWater,
        ItemBoxItem_ShieldFlame,
        ItemBoxItem_ShieldLightning,
        ItemBoxItem_Invincibility,
        ItemBoxItem_SpeedShoes,
        ItemBoxItem_LifeSonic,
        ItemBoxItem_LifeTails,
        ItemBoxItem_LifeKnuckles,
        ItemBoxItem_Eggman,
        ItemBoxItem_CombineRing,
        ItemBoxItem_TeamSwap,
        ItemBoxItem_Random,
        ItemBoxItem_Super,
        ItemBoxItem_LifeMighty,
        ItemBoxItem_LifeRay
    };

    enum CollisionModes : byte
    {
        CollisionMode_Floor,
        CollisionMode_LeftWall,
        CollisionMode_Roof,
        CollisionMode_RightWall
    };

    // TODO: needs updating
    enum ObjectType
    {
        //ObjectType_SmogHandle       = 0x00E48CE8,
        //ObjectType_OctusB           = 0x00E48E1C,
        //ObjectType_MechaBu          = 0x00E48FDC,
        //ObjectType_Iwamodoki        = 0x00EBB5D0,
        //ObjectType_Grabber          = 0x00EBB5E0,
        //ObjectType_Woodrow          = 0x00EBB60C,
        //ObjectType_BallHog          = 0x00EBB610,
        //ObjectType_Jugglesaw        = 0x00EBB620,
        //ObjectType_Bumpalo          = 0x00EBB63C,
        //ObjectType_Blastoid         = 0x00EBB68C,
        //ObjectType_Spiny            = 0x00EBB69C,
        //ObjectType_FlasherMkII      = 0x00EBB6A4,
        //ObjectType_Bulb             = 0x00EBB6B0, // TODO: Find proper name
        //ObjectType_Buggernaut       = 0x00EBB6C0,
        //ObjectType_CheckPoint       = 0x00EBB6C4,
        //ObjectType_Rattlekiller     = 0x00EBB6E0,
        //ObjectType_MonkeyDude       = 0x00EBB71C,
        //ObjectType_CatakillerJR     = 0x00EBB730,
        //ObjectType_Sol              = 0x00EBB744,
        //ObjectType_Vultron          = 0x00EBB764,
        //ObjectType_Ring             = 0x00EBB778,
        //ObjectType_DDWrecker        = 0x00EBB788,
        //ObjectType_TurboTurtle      = 0x00EBB7D4,
        //ObjectType_SSRing           = 0x00EBB804,
        //ObjectType_Sweep            = 0x00EBB808,
        //ObjectType_Turtloid         = 0x00EBB83C,
        //ObjectType_FlameThrower     = 0x00EBB840,
        //ObjectType_DoorSwitch       = 0x00EBB854,
        //ObjectType_Splats           = 0x00EBB898,
        //ObjectType_Dragonfly        = 0x00EBB8C0,
        //ObjectType_PohBee           = 0x00EBB8E8,
        //ObjectType_Roller           = 0x00EBB938,
        //ObjectType_Scarab           = 0x00EBB940,
        //ObjectType_LightBulb        = 0x00EBB94C,
        //ObjectType_Rhinobot         = 0x00EBB954,
        //ObjectType_RockDrill        = 0x00EBB960,
        //ObjectType_TechnoSqueek     = 0x00EBB994,
        //ObjectType_Pointdexter      = 0x00EBB9C4,
        //ObjectType_Fireworm         = 0x00EBB9D8,
        //ObjectType_Blaster          = 0x00EBBA04,
        //ObjectType_Chopper          = 0x00EBBA24,
        //ObjectType_Kabasira         = 0x00EBBA30,
        //ObjectType_Tubinaut         = 0x00EBBA68,
        //ObjectType_WallCrawl        = 0x00EBBAAC,
        //ObjectType_Motobug          = 0x00EBBAF8,
        //ObjectType_Jellygnite       = 0x00EBBB34,
        //ObjectType_Act1SkyBoss      = 0x00EBBB44, // TODO: Find proper name
        //ObjectType_Spring           = 0x00EBBB70,
        //ObjectType_Effect           = 0x00EBBB8C,
        //ObjectType_MatryoshkaBom    = 0x00EBBBC8,
        //ObjectType_DrillerDroidB    = 0x00EBBBD4,
        //ObjectType_MeterDroidB      = 0x00EBBBF0,
        //ObjectType_Clucker          = 0x00EBBC20,
        //ObjectType_Pinata           = 0x00EBBC94,
        //ObjectType_Jawz             = 0x00EBBCA8,
        //ObjectType_Hotaru           = 0x00EBBCC0,
        //ObjectType_DERobot          = 0x00EBBD08,
        //ObjectType_Animal           = 0x00EBBD10,
        //ObjectType_ScoreOnHitNumber = 0x00EBBD18,
        //ObjectType_Explosion        = 0x00EBBD1C,
        //ObjectType_MicDrop          = 0x00EBBD40,
        //ObjectType_MegaChopper      = 0x00EBBD50,
        //ObjectType_Octus            = 0x00EBBD5C,
        //ObjectType_Kanabun          = 0x00EBBD64,
        //ObjectType_Rexon            = 0x00EBBD6C,
        //ObjectType_TurboSpiker      = 0x00EBBD88,
        //ObjectType_ShutterBug       = 0x00EBBDF0,
        //ObjectType_IceBomba         = 0x00EBBDF4,
        //ObjectType_SentryBug        = 0x00EBBE08,
        //ObjectType_Cactula          = 0x00EBBE5C,
        //ObjectType_Aquis            = 0x00EBBE7C,
        //ObjectType_ItemBox          = 0x00EBBE98,
        //ObjectType_BuzzBomber       = 0x00EBBEC4,
        //ObjectType_Dango            = 0x00EBBECC,
        //ObjectType_Bloominator      = 0x00EBBEE0,
        //ObjectType_NewTron          = 0x00EBBF1C,
        //ObjectType_CaveBat          = 0x00EBBF28,
        //ObjectType_CrabMeat         = 0x00EBBF34,
        //ObjectType_Toxomister       = 0x00EBBF38
        //General
        ObjectType_Player = 0x00AC6838,
        ObjectType_Ring = 0x00AC67E0,
        ObjectType_Platform = 0x00AC6CB4,
        ObjectType_SSRing = 0x00AC686C,
        ObjectType_ItemBox = 0x00AC6F00,
        ObjectType_Spring = 0x00AC6BD8,
        ObjectType_Spikes = 0x00AC67BC,
        ObjectType_Button = 0x00AC68BC,
        ObjectType_Door = 0x00AC68B0,
        ObjectType_LRZBreakableWall = 0x00AC6C80,
        ObjectType_BreakableWall = 0x00AC6A34,
        ObjectType_CollapsingPlatform = 0x00AC66EC,
        ObjectType_ForcedSpin = 0x00AC6CB4,
        ObjectType_StarPost = 0x00AC672C,
        ObjectType_Animals = 0x00AC6D78,
        ObjectType_ScoreBonus = 0x00AC6D80,
        ObjectType_Explosion = 0x00AC6D84,
        ObjectType_PlathSwapper = 0x00AC6C0C,

        // Green Hill Zone
        ObjectType_Motobug = 0x00AC6B60,
        ObjectType_CrabMeat = 0x00AC6F9C,
        ObjectType_Chopper = 0x00AC6A8C,
        ObjectType_BuzzBomber = 0x00AC6F2C,
        ObjectType_NewTron = 0x00AC6F84,
        ObjectType_BatBrain = 0x00AC6F90,
        ObjectType_Splats = 0x00AC6900,
        ObjectType_DDWrecker = 0x00AC67F0, //Boss Act 1
        ObjectType_DERobot = 0x00AC6D70, //Boss Act 2
        // Chemical Plant Zone
        ObjectType_Grabber = 0x00AC6648,
        ObjectType_Spiny = 0x00AC6704,
        ObjectType_AmoebaDroid = 0x00AC6880, //Boss Act 1
        ObjectType_Sweep = 0x00AC6870,
        ObjectType_CatakillerJR = 0x00AC6798,
        // Studiopolis Zone
        ObjectType_MicDrop = 0x00AC6DA8,
        ObjectType_ShutterBug = 0x00AC6E58,
        ObjectType_Tubinaut = 0x00AC6AD0,
        ObjectType_WallCrawl = 0x00AC6B14,
        ObjectType_HeavyGunner = 0x00AC6C2C, //Boss Act 1
        ObjectType_WeatherMobile = 0x00AC6A80, //Boss Act 2
        ObjectType_CircleBumper = 0x00AC6904,
        // Flying Battery
        ObjectType_TechnoSqueek = 0x00AC69FC,
        ObjectType_Blaster = 0x00AC6A6C,
        ObjectType_Clucker = 0x00AC6C88,
        // Press Garden Zone
        ObjectType_Dragonfly = 0x00AC6928,
        ObjectType_Jugglesaw = 0x00AC6688,
        ObjectType_SplatsJar = 0x004ACF50,
        ObjectType_Woodrow = 0x00AC6674,
        ObjectType_IceBomba = 0x00AC6E5C,
        ObjectType_HeavyShinobi = 0x00AC6B08,
        // StarDust Speed Way
        ObjectType_Hotaru = 0x00AC6808,
        ObjectType_Dango = 0x00AC6F34,
        ObjectType_Kanabun = 0x00AC67FC,
        ObjectType_Kabasira = 0x004ACF50,
        ObjectType_SilverSonic = 0x00AC679C,
        ObjectType_MetalSonic = 0x00AC6B34, //Boss Act2 M
        // HydroCity Zone
        ObjectType_Jellygnite = 0x00AC6B9C,
        ObjectType_Pointdexter = 0x00AC6A2C,
        ObjectType_Blastoid = 0x00AC66F4,
        ObjectType_Jawz = 0x00AC6D10,
        ObjectType_MovingDust = 0x00AC6D18, // Mighty - Ground Pound
        ObjectType_MegaChopper = 0x00AC6DB8,
        ObjectType_TurboSpiker = 0x00AC6DF0,
        ObjectType_Buggernaut = 0x00AC6728,
        //MirageSaloonZone
        ObjectType_Vultron = 0x00AC67CC,
        ObjectType_Cactula = 0x00AC6EC4,
        ObjectType_Bumpalo = 0x00AC66A4,
        ObjectType_Rattlekiller = 0x00AC6748,
        ObjectType_BallHog = 0x00AC69A0,
        ObjectType_Armadiloid = 0x00AC68A4,
        //Oil Ocean Zone
        ObjectType_Aquis = 0x00AC6EE4,
        ObjectType_Octus = 0x00AC6DC4,
        ObjectType_Sol = 0x00AC6778,
        // Lave Reef Zone
        ObjectType_Toxomister = 0x00AC6FA0,
        ObjectType_Fireworm = 0x00AC6A40,
        ObjectType_Iwamodoki = 0x00AC6638,
        ObjectType_Rexon = 0x00AC6DD4,
        // Metallic Madness Zone
        ObjectType_MechaBu = 0x00AC6888,
        ObjectType_MatryoshkaBom = 0x00AC6C30,
        ObjectType_Scarab = 0x00AC69A8,
        ObjectType_PohBee = 0x00AC6950,
        // Titanic Monarch Zone
        ObjectType_SentryBug = 0x00AC6E70,
        ObjectType_TurboTurtle = 0x00AC683C,
        ObjectType_FlasherMkII = 0x00AC670C,
        ObjectType_BallHog2 = 0x00AC6678,

        // Angel Island Zone 
        ObjectType_MonkeyDude = 0x00AC6784,
        ObjectType_Rhinobot = 0x00AC6C24,
        ObjectType_Bloominator = 0x00AC6F48,

        //Unused - Other
        ObjectType_S1Orbinaught = 0x00AC6A00,
        ObjectType_S1Catakiller = 0x00AC6B78,

        //Service Objects 
        ObjectType_FXFade = 0x00AC6810,

        //UFO - SpecialStage
        ObjectType_UFO_Dust = 0x00AC6DB0,
        ObjectType_UFO_Itembox = 0x00AC66B4,
        ObjectType_SpecialClear = 0x00AC6CC8,
        ObjectType_UFO_Decoration = 0x00AC6DB0,
        ObjectType_UFO_Springboard = 0x00AC67D4,
        ObjectType_UFO_Message = 0x00AC6EBC,
        ObjectType_UFO_Sphere = 0x00AC667C,
        ObjectType_UFO_Ring = 0x00AC6700,
        ObjectType_UFO_Player = 0x00AC6F8C,

        //UI - 
        ObjectType_UIWaitSpinner = 0x00AC6DA0,
        ObjectType_PauseMenu = 0x00AC6EF0,
        ObjectType_FXRuby = 0x00AC6E90,

        //System
        ObjectType_DebugMode = 0x00AC6930
    };

    enum TransparencyFlag : byte
    {
        TransparencyFlag_Opaque,
        TransparencyFlag_HalfTransparent,
        TransparencyFlag_Transparent
    };

    enum SuperState : int
    {
        SuperState_NotActive,
        SuperState_Activate,
        SuperState_Active,
        SuperState_Deactivate
    };

#pragma endregion

    // TODO: needs updating
#pragma region Constents

// Player Status
#define CONTROL_NONE                  (SonicMania::PlayerStatus)(0x00000000)
#define CONTROL_NORMAL                (SonicMania::PlayerStatus)(baseAddress + 0x000C3E00)
#define CONTROL_P2                    (SonicMania::PlayerStatus)(baseAddress + 0x000C4340)


// Player Status
#define PLAYERSTATE_NONE              (SonicMania::PlayerStatus)(0x00000000)
#define PLAYERSTATE_GROUND            (SonicMania::PlayerStatus)(baseAddress + 0x000CAD80)
#define PLAYERSTATE_AIR               (SonicMania::PlayerStatus)(baseAddress + 0x000CB6C0)
#define PLAYERSTATE_JUMP              (SonicMania::PlayerStatus)(baseAddress + 0x000CB6C0)
#define PLAYERSTATE_LOOKDOWN          (SonicMania::PlayerStatus)(baseAddress + 0x000CBBD0)
#define PLAYERSTATE_LOOKUP            (SonicMania::PlayerStatus)(baseAddress + 0x000CBAA0)
#define PLAYERSTATE_SPINDASH          (SonicMania::PlayerStatus)(baseAddress + 0x000CBD80)
#define PLAYERSTATE_Roll              (SonicMania::PlayerStatus)(baseAddress + 0x000CB850)
#define PLAYERSTATE_HURT              (SonicMania::PlayerStatus)(baseAddress + 0x000CCA00)
#define PLAYERSTATE_DIE               (SonicMania::PlayerStatus)(baseAddress + 0x000CCAC0)
#define PLAYERSTATE_OUTTAHERE         (SonicMania::PlayerStatus)(baseAddress + 0x000CCE90)
#define PLAYERSTATE_Gimmick           (SonicMania::PlayerStatus)(baseAddress + 0x000CAD70)
#define PLAYERSTATE_Gimmick2          (SonicMania::PlayerStatus)(baseAddress + 0x000CBA90) // TODO: needs updating
#define PLAYERSTATE_Hanging           (SonicMania::PlayerStatus)(baseAddress + 0x000CAD70) // TODO: needs updating
#define PLAYERSTATE_SpiningTube_PGZ   (SonicMania::PlayerStatus)(baseAddress + 0x0001F7F0) // TODO: needs updating
#define PLAYERSTATE_Frozen_PGZ        (SonicMania::PlayerStatus)(baseAddress + 0x000A3410) // TODO: needs updating
#define PLAYERSTATE_Sliding_OOZ       (SonicMania::PlayerStatus)(baseAddress + 0x0002EB10) // TODO: needs updating
#define PLAYERSTATE_SwitchingToBG_MMZ (SonicMania::PlayerStatus)(baseAddress + 0x000304A0) // TODO: needs updating
#define PLAYERSTATE_SwitchingToFG_MMZ (SonicMania::PlayerStatus)(baseAddress + 0x000305B0) // TODO: needs updating
#define PLAYERSTATE_TurningSmall_MMZ  (SonicMania::PlayerStatus)(baseAddress + 0x00031560) // TODO: needs updating
#define PLAYERSTATE_TurningBig_MMZ    (SonicMania::PlayerStatus)(baseAddress + 0x00031690) // TODO: needs updating
#define PLAYERSTATE_P2FLYIN           (SonicMania::PlayerStatus)(baseAddress + 0x000CD150)
#define PLAYERSTATE_P2JUMPIN          (SonicMania::PlayerStatus)(baseAddress + 0x000CD560) 
#define PLAYERSTATE_SpringBasic       (SonicMania::PlayerStatus)(baseAddress + 0x000CB6C0)
#define PLAYERSTATE_ScoreCard         (SonicMania::PlayerStatus)(baseAddress + 0x000CCD30)
#define PLAYERSTATE_TransportTube_CPZ (SonicMania::PlayerStatus)(baseAddress + 0x000CBA90)
#define PLAYERSTATE_HCZSlide          (SonicMania::PlayerStatus)(baseAddress + 0x000B9970); //Added from LHPZ Mod


// Sonic
#define PLAYERSTATE_DROPDASH          (SonicMania::PlayerStatus)(baseAddress + 0x000CC1F0)
#define PLAYERSTATE_PEELOUT           (SonicMania::PlayerStatus)(baseAddress + 0x000CBF70)

// Tails
#define PLAYERSTATE_FLYING            (SonicMania::PlayerStatus)(baseAddress + 0x000CCF30)
#define PLAYERSTATE_FLYCARRIED        (SonicMania::PlayerStatus)(baseAddress + 0x000CDA60)

// Knuckles
#define PLAYERSTATE_GLIDELEFT         (SonicMania::PlayerStatus)(baseAddress + 0x000CDB10)
#define PLAYERSTATE_GLIDERIGHT        (SonicMania::PlayerStatus)(baseAddress + 0x000CDE10)
#define PLAYERSTATE_GLIDESLIDE        (SonicMania::PlayerStatus)(baseAddress + 0x000CE230)
#define PLAYERSTATE_CLIMB             (SonicMania::PlayerStatus)(baseAddress + 0x000CE380)
#define PLAYERSTATE_LEDGEPULLUP       (SonicMania::PlayerStatus)(baseAddress + 0x000CE6F0)
#define PLAYERSTATE_GLIDEDROP         (SonicMania::PlayerStatus)(baseAddress + 0x000CE110)

// Mighty
#define PLAYERSTATE_HAMMERDROP        (SonicMania::PlayerStatus)(baseAddress + 0x000CC580)
#define PLAYERSTATE_UNSPIN            (SonicMania::PlayerStatus)(baseAddress + 0x000CC990) //Unspin after bouncing on hazard

// Ray
#define PLAYERSTATE_RAYGLIDE          (SonicMania::PlayerStatus)(baseAddress + 0x000CD660)

// Move Sets
#define MOVESET_NONE    (SonicMania::Ability)(0x00000000)
#define MOVESET_SONIC   (SonicMania::Ability)(baseAddress + 0x000C8630)
#define MOVESET_TAILS   (SonicMania::Ability)(baseAddress + 0x000C8990)
#define MOVESET_KNUX    (SonicMania::Ability)(baseAddress + 0x000C8A70)
#define MOVESET_MIGHTY  (SonicMania::Ability)(baseAddress + 0x000C8B70)
#define MOVESET_RAY     (SonicMania::Ability)(baseAddress + 0x000C8DF0)
#define MOVESET_ERSS    (SonicMania::Ability)(baseAddress + 0x000C2340) // Egg Reverie Super Sonic
#define UpAbility_Peelout    (SonicMania::Ability)(baseAddress + 0x000C8FF0)


#define PLAYERID1 0
#define PLAYERID2 1
#define PLAYERID3 2
#define PLAYERID4 3

//not a state but calling this will hit a player, it has no checks, not even for invincible frames
    ThiscallFunctionPointer(int, Player_Hit, (EntityPlayer* Player), 0x000C5970);

#pragma endregion

#pragma region Structs

    struct Vector2
    {
        Vector2()
        {

        }

        Vector2(int x, int y) : Vector2()
        {
            X = x;
            Y = y;
        }

        Vector2(float x, float y) : Vector2()
        {
            X = (short)x;
            Y = (short)y;
            SubX = (ushort)((x - (ushort)(x)) * 0x10000);
            SubX = (ushort)((y - (ushort)(y)) * 0x10000);
        }

        Vector2(int x) : Vector2()
        {
            X = x;
            Y = x;
        }

        Vector2* Add(Vector2& vec)
        {
            X += vec.X;
            Y += vec.Y;
            return this;
        }

        Vector2* Sub(Vector2& vec)
        {
            X -= vec.X;
            Y -= vec.Y;
            return this;
        }

        Vector2* Mul(Vector2& vec)
        {
            X *= vec.X;
            Y *= vec.Y;
            return this;
        }

        Vector2* Div(Vector2& vec)
        {
            X /= vec.X;
            Y /= vec.Y;
            return this;
        }

        Vector2* Abs()
        {
            if (X < 0)
                X *= -1;
            if (Y < 0)
                Y *= -1;
        }

        Vector2 operator+(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Add(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator+=(const Vector2& vec2)
        {
            Add(*(Vector2*)&vec2);
            return *this;
        }

        Vector2 operator-(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Sub(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator-=(const Vector2& vec2)
        {
            Sub(*(Vector2*)&vec2);
            return *this;
        }

        Vector2 operator*(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Mul(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator*=(const Vector2& vec2)
        {
            Mul(*(Vector2*)&vec2);
            return *this;
        }

        Vector2 operator/(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Div(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator/=(const Vector2& vec2)
        {
            Div(*(Vector2*)&vec2);
            return *this;
        }

        /** Calculates the Angle to the target vector
            Vector2 target: Location to the target.
            double Return: Angle between the main and target in Radians.
        **/
        double CalculateAngle(Vector2 target)
        {
            double theta = atan2(target.X - X, Y - target.Y);
            if (theta < 0.0)
                theta += 6.2831853071795865;
            return theta;
        }

        /** Calculates the Distance between to the target vector and the main vector
            Vector2 target: Location to the target.
            double Return: The Distance between the main and target.
        **/
        double inline CalculateDistance(Vector2 target)
        {
            return sqrt(pow(X - target.X, 2) + pow(Y - target.Y, 2));
        }

        int GetFullX()
        {
            return (X << 16) + SubX;
        }

        int GetFullY()
        {
            return (Y << 16) + SubY;
        }

        float GetFullX_F()
        {
            return (float)X + (float)(SubX / 0x10000);
        }

        float GetFullY_F()
        {
            return (float)Y + (float)(SubY / 0x10000);
        }

        void SetFullX(int x)
        {
            SubX = (short)x;
            X = x >> 16;
        }

        void SetFullY(int y)
        {
            SubY = (short)y;
            Y = y >> 16;
        }

        void SetFullX(float x)
        {
            X = (short)x;
            SubX = (ushort)((x - (ushort)(x)) * 0x10000);
        }

        void SetFullY(float y)
        {
            Y = (short)y;
            SubY = (ushort)((y - (ushort)(y)) * 0x10000);
        }

        bool inline IsNull()
        {
            return X == 0 && Y == 0;
        }

    public:
        ushort SubX = 0;
        short X = 0;

        ushort SubY = 0;
        short Y = 0;
    };
    struct Color
    {
        byte Red = 0;
        byte Green = 0;
        byte Blue = 0;

        Color()
        {

        }

        Color(int rgb)
        {
            Red = (rgb >> 16) & 0xFF;
            Green = (rgb >> 8) & 0xFF;
            Blue = rgb & 0xFF;
        }

        inline SHORT ToRGB565()
        {
            //return ((Red & 0b11111000) << 8) | ((Green & 0b11111100) << 3) | (Blue >> 3);
            return (Blue >> 3) | 32 * (Green >> 2) | ((Red >> 3) << 11);
        }

        inline int ToRGB888()
        {
            return ((Red & 0xFF) << 16) + ((Green & 0xFF) << 16) + (Blue & 0xFF);
        }

        inline void FromRGB565(SHORT RGB565)
        {
            Red = (RGB565 & 0b1111100000000000) >> 8;
            Green = (RGB565 & 0b0000011111100000) >> 3;
            Blue = (RGB565 & 0b0000000000011111) << 3;
        }

        inline void FromINT(int rgb)
        {
            Red = (rgb >> 16) & 0xFF;
            Green = (rgb >> 8) & 0xFF;
            Blue = rgb & 0xFF;
        }

        inline void Tint(float tint)
        {
            Red = (byte)(Red * tint);
            Green = (byte)(Green * tint);
            Blue = (byte)(Blue * tint);
        }

        inline void Blend(byte r, byte g, byte b, float a)
        {
            Red = (byte)(a * r + (1 - a) * Red);
            Green = (byte)(a * g + (1 - a) * Green);
            Blue = (byte)(a * b + (1 - a) * Blue);
        }

        inline void Blend(Color color, float a)
        {
            Red = (byte)(a * color.Red + (1 - a) * Red);
            Green = (byte)(a * color.Green + (1 - a) * Green);
            Blue = (byte)(a * color.Blue + (1 - a) * Blue);
        }



    };
    struct ControllerInput
    {
        /* 0x00000000 */ BOOL Down;
        /* 0x00000004 */ BOOL Press;
        /* 0x00000008 */ DWORD Key;
    };
    struct Controller
    {
        /* 0x00000000 */ ControllerInput Up;
        /* 0x0000000C */ ControllerInput Down;
        /* 0x00000018 */ ControllerInput Left;
        /* 0x00000024 */ ControllerInput Right;
        /* 0x00000030 */ ControllerInput A;
        /* 0x0000003C */ ControllerInput B;
        /* 0x00000048 */ ControllerInput C;
        /* 0x00000054 */ ControllerInput X;
        /* 0x00000060 */ ControllerInput Y;
        /* 0x0000006C */ ControllerInput Z;
        /* 0x00000078 */ ControllerInput Start;
        /* 0x00000084 */ ControllerInput Select;
    };
    struct __declspec(align(4)) EntityAnimationData
    {
        /* 0x00000000 */ void* Animationptr;
        /* 0x00000004 */ int CurrentFrame;
        /* 0x00000008 */ short CurrentAnimation;
        /* 0x0000000A */ short LastAnimation;
        /* 0x0000000C */ short Speed;
        /* 0x0000000E */ short Unknown0E;
        /* 0x00000010 */ short Duration;
        /* 0x00000012 */ short FrameCount; // Frame count + 1?
        /* 0x00000014 */ BYTE unknown14;
        /* 0x00000015 */ BYTE UserFrameCount; // I Added This Normally Unknown
    };
    struct struct_Timer
    { // Not a proper struct, I just want the timer
        /* 0x00000000 */ bool Enabled;
        /* 0x00000001 */ byte Other[7];
        /* 0x00000008 */ byte TimerCentisecond;
        /* 0x00000009 */ byte TimerSecond;
        /* 0x0000000A */ byte TimerMinute;

        void ResetTimer()
        {
            TimerMinute = TimerSecond = TimerCentisecond = 0;
        }

        void AddTime(byte centiseconds = 0, byte seconds = 0, byte minutes = 0)
        {
            TimerMinute += minutes;
            TimerSecond += seconds;
            TimerCentisecond += centiseconds;
            while (TimerCentisecond > 100)
            {
                TimerCentisecond -= 100;
                ++TimerSecond;
            }
            while (TimerSecond > 60)
            {
                TimerSecond -= 60;
                ++TimerMinute;
            }
        }
    };

    struct Hitbox
    {
        short Left;
        short Top;
        short Right;
        short Bottom;
    };

    struct CollisionSensor
    {
        int XPos;
        int YPos;
        int Collided;
        int Angle;
    };

    struct SensorInfo
    {
        CollisionSensor Sensors[5];
    };

    struct Matrix
    {
        int Values[16];
    };

    struct ActiveEntityInfo
    {
        EntityBase* CurrentEntity;
        DWORD field_4;
        DWORD field_8;
        DWORD field_C;
        DWORD field_10;
        DWORD field_14;
        DWORD field_18;
        DWORD field_1C;
        DWORD field_20;
        DWORD field_24;
        DWORD field_28;
        DWORD field_2C;
        DWORD field_30;
        DWORD field_34;
    };

#pragma endregion

#pragma region Functions

    //New
    FastcallFunctionPointer(bool, Player_CheckBadnikHit, (EntityPlayer* Player, Entity* Entity, Hitbox* EntityHitbox), 0x000C5E30);
    FunctionPointer(int, CheckObjectCollisionTouch, (Entity* ThisEntity, Hitbox* ThisHitbox, Entity* OtherEntity, Hitbox* OtherHitbox), 0x001BEB20);
    FunctionPointer(int, CheckObjectCollisionBox, (Entity* ThisEntity, Hitbox* ThisHitbox, Entity* OtherEntity, Hitbox* OtherHitbox), 0x001BEDD0);
    FunctionPointer(int, CheckObjectCollisionPlatform, (Entity* ThisEntity, Hitbox* ThisHitbox, Entity* OtherEntity, Hitbox* OtherHitbox), 0x001BF330);
    FunctionPointer(Hitbox*, GetHitbox, (EntityAnimationData* AnimData, int HitboxID), 0x001B3930);
    FunctionPointer(void, ProcessAnimation, (EntityAnimationData* AnimData), 0x001B3A70);
    FunctionPointer(int, Camera_ShakeScreen, (int ShakeX, int EntityID, int ShakeY), 0x002140); //Broken, Use "ShakeCamera"
    FunctionPointer(int, Camera_SetTargetEntity, (int EntityID, Entity* Target), 0x002010); //Untested, may work
    FunctionPointer(int, GetEntityID, (Entity* EntityPtr), 0x001D3CC0);
    FunctionPointer(int, GetActiveObjects, (unsigned __int16 ObjectType, Entity* EntityPtr), 0x001C8430);
    FunctionPointer(int, GetObjects, (unsigned __int16 ObjectType, Entity* EntityPtr), 0x001C84E0);
    FunctionPointer(void*, GetObjectByID, (unsigned __int16 SlotID), 0x001D3C90);
    FunctionPointer(int, GetAttribute, (int AttributeType, char* AttributeName, int ObjectID, int StoreOffset), 0x001D3B20);
    FunctionPointer(char, GetTileAngle, (__int16 TileXPos, unsigned __int8 TileYPos, char CollisionMode), 0x001C22A0);
    FunctionPointer(char, GetTileBehaviour, (__int16 TileXPos, unsigned __int8 TileYPos), 0x001C23C0);
    FastcallFunctionPointer(int, Itembox_Break, (EntityItemBox* ItemBox, EntityPlayer* Player), 0xA9930);

    //Unknown Function Ptrs (that are used and needed
    FunctionPointer(int, RSDK_Unknown45, (), 0x00AA7744);

    // Audio
    FastcallFunctionPointer(void, SetupMusic, (int slot, const char* filePath, int loopStart), 0x00002AD0);
    FunctionPointer(int, PlayMusic, (const char* filePath, int slot, int a3, int loopstart, bool loop), 0x001BC640);
    FunctionPointer(int, PlaySoundFX, (short SoundFXID, signed int a2, unsigned __int8 a3), 0x001BC390);
    FunctionPointer(short, GetSoundFXID, (const char* path), 0x001BC2F0);
    FunctionPointer(void, ChangeMusicSpeed, (int slot, float volume, float Channelbalance, float PlaybackSpeed), 0x001BC830);

    // Entity
    FunctionPointer(bool, SetSpriteAnimation, (short spriteIndex, short animationID, EntityAnimationData* animationData, bool forceApply, short frameID), 0x001B35E0);
    FastcallFunctionPointer(void, FastChangeCharacter, (EntityPlayer* player, Character character), 0x000C7920);
    FastcallFunctionPointer(void, ApplyShieldEffect, (Entity* entity), 0x000C5910);
    FunctionPointer(Entity*, SpawnObject_Internal, (short objectID, short subObject, DWORD x, DWORD y), 0x001D3E00);
    FunctionPointer(void, DespawnEntity_Internal, (Entity* entity, DWORD a2, DWORD a3), 0x001D3CF0);
    FunctionPointer(FILE*, LoadStaticObject, (int ObjStruct, int a2, Scope scope), 0x001D32B0);
    //Collision
    FunctionPointer(int, ObjectTileCollision, (Entity* Entity, unsigned __int16 CollisionLayers, char CollisionMode, char CollisionPlane, int XOffset, int YOffset, int SetPos), 0x001BF5F0); //Check Collision at a certain point
    FunctionPointer(int, ObjectPathGrip, (Entity* Entity, unsigned __int16 CollisionLayers, char CollisionMode, char CollisionPlane, int XOffset, int YOffset, int SetPos), 0x001BFB40); //Check Collision around a certain point
    FunctionPointer(void, ProcessPlayerTileCollisions, (EntityPlayer* Player, Hitbox* OuterBox, Hitbox* InnerBox), 0x001C0060);

    // Graphics
    //FunctionPointer(void, DrawRect, (short XPos, short YPos, short SizeY, short SizeX, byte R, byte G, byte B, byte Alpha), 0x00001DAC10);
    FunctionPointer(void, DrawRect, (int Xpos, int Ypos, int Width, int Height, int Colour, signed int Alpha, InkEffect InkEffect, BOOL ScreenRelative), 0x001D8870);
    FunctionPointer(void, DrawVertices, (Vector2* Verticies, int VertCount, int Red, int Green, int Blue, int Alpha, InkEffect InkEffect), 0x1DAC10);
    FunctionPointer(void, DrawCircle, (int Xpos, int Ypos, signed int Radius, int Colour, signed int Alpha, InkEffect InkEffect, BOOL ScreenRelative), 0x1D9890);
    FunctionPointer(void, DrawCircleOutline, (int Xpos, int Ypos, signed int InnerRadius, signed int OuterRadius, int Colour, signed int Alpha, InkEffect InkEffect, BOOL ScreenRelative), 0x1DA170);
    FunctionPointer(void*, LoadAniTiles, (const char* FilePath, Scope scope), 0x001D4CE0);
    FunctionPointer(void*, SetAniTiles, (ushort SheetID, ushort TileIndex, ushort SrcX, ushort SrcY, ushort FrameWidth, ushort FrameHeight), 0x00BDC4B0);
    FunctionPointer(void*, LoadMesh, (const char* filepath, Scope scope), 0x00BDE080);
    FunctionPointer(int, DrawSprite, (EntityAnimationData* AnimData, Vector2* Position, BOOL ScreenRelative), 0x001B3B00);
    FunctionPointer(void, DrawLine, (int X1, int Y1, int X2, int Y2, unsigned int Colour, signed int Alpha, int InkEffect, BOOL ScreenRelative), 0x001D8DF0);
    FunctionPointer(void*, DrawSpriteRotoZoom, (int XPos, int YPos, int PivotX, int PivotY, int Width, int Height, int SprX, int SprY, signed int ScaleX, signed int ScaleY, int Direction, __int16 Rotation, int InkEffect, signed int Alpha, int SheetID), 0x001D7260); //Internal Call From "DrawSprite"
    FunctionPointer(void*, DrawMesh, (ushort AnimationID, ushort ModelID, char a3, DWORD* a4, DWORD* a5, int Colour), 0x001DEF00);
    FunctionPointer(short, LoadAnimation, (const char* filename, Scope scope), 0x001B1FA0);
    FunctionPointer(char, LoadGif, (int a1, char* filepath, int buffer), 0x001CBA90);
    //Palettes
    FunctionPointer(int, SetPaletteEntry, (char PaletteID, unsigned char Index, int Color), 0x001D5020);
    FunctionPointer(int, GetPaletteEntry, (char PaletteID, unsigned char Index), 0x001D5070);
    FunctionPointer(void, BlendFromPalettes, (byte DestPaletteID, byte SourcePalA, byte SourcePalB, signed int AlphaPerFrame, int StartIndex, int EndIndex), 0x001D54E0);
    FunctionPointer(void*, SetActivePalette, (byte PaletteID, int StartLine, int EndLine), 0x001D50B0);
    FunctionPointer(void*, CopyPaletteColors, (byte SourcePalID, byte SourceIndex, byte DestPalID, byte DestIndex, byte ColorCount), 0x001D50F0);
    FunctionPointer(void*, LoadPalette, (byte PaletteID, const char* Filename, unsigned short ColorCount), 0x001D5150);
    FunctionPointer(int, RotatePalette, (unsigned __int8 PaletteID, unsigned __int8 StartIndex, unsigned __int8 EndIndex, bool Right), 0x001D5420);
    FunctionPointer(int, SetPaletteMask, (int MaskColour), 0x001D4FE0);
    FunctionPointer(int, SetInkColour, (int Value), 0x001D4A50);

    // Utility
    FunctionPointer(int, Rand, (int Min, int Max), 0x001DCDA0);
    FunctionPointer(int, GetSettingsValue, (int ValueID), 0x001E3270);
    FunctionPointer(void*, SetSettingsValue, (int ValueID, signed int Value), 0x001E33A0);
    FunctionPointer(void, LoadSettings, (), 0x001CD690);
    FunctionPointer(void, SaveSettings, (bool writeToFile), 0x005E4780);
    FunctionPointer(void*, SetPresence, (int a1, const char* message), 0x002016D0);

    // Scenes
    FunctionPointer(int, LoadSceneFile, (int a1), 0x001EF0C0);
    FunctionPointer(void, LoadScene, (int a1), 0x001EF0C0);
    FunctionPointer(int, LoadGameScene, (char* CategoryName, char* SceneName), 0x001F23C0);
    FunctionPointer(char, LoadTileconfig, (), 0x001BE440);
    FunctionPointer(void, LoadGameconfig, (int ths), 0x001C6510);
    FunctionPointer(int, GetSceneLayerID, (const char* name), 0x001E1600);
    FunctionPointer(void*, GetSceneLayerInfo, (unsigned __int16 LayerID), 0x001E16B0); //Actually returns SceneLayer* but thats not added yet
    FunctionPointer(__int16, GetLayerSize, (unsigned __int16 LayerID, Vector2* Size, DWORD PixelSize), 0x001E16E0);

    // MATRICIES - Untested
    FunctionPointer(void*, MatrixTranslateXYZ, (Matrix* Matrix, int X, int Y, int Z), 0x001DD3F0);
    FunctionPointer(void*, MatrixRotateX, (Matrix* Matrix, ushort RotationX), 0x001DD470);
    FunctionPointer(void*, MatrixRotateY, (Matrix* Matrix, ushort RotationY), 0x001DD500);
    FunctionPointer(void*, MatrixRotateZ, (Matrix* Matrix, ushort RotationZ), 0x001DD590);
    FunctionPointer(void*, MatrixInvert, (unsigned int a1, Matrix* Matrix), 0x001DD770);
    FunctionPointer(void*, MatrixMultiply, (Matrix* Matrix1, Matrix* Matrix2), 0x001DE010);



#pragma endregion

#pragma region DataPointers

    // Players
    //DataPointer(EntityBase, ObjectList, 0x00469A10, 0x940); //Object List (Size: 0x940)
    DataArray(Entity, ObjectEntityList, 0x00469A10, 0x940);

    DataPointer(EntityPlayer, Player1, 0x00469A10);
    DataPointer(EntityPlayer, Player2, 0x00469E68);
    DataPointer(EntityPlayer, Player3, 0x0046A2C0);
    DataPointer(EntityPlayer, Player4, 0x0046A718);

    //Probably is just 0,0,0,0 (used as default for player if OuterBox/InnerBox isn't found)
    DataPointer(Hitbox, DefaultHitbox, 0x0026B804);

    //Currently Running Entity
    DataPointer(ActiveEntityInfo*, EntityInfo, 0x00AA7634);

    //Audio/Sound
    DataPointer(float, StreamVolume, 0xA53078);
    DataPointer(float, SFXVolume, 0xA5307C);

    // Other
    DataPointer(Scene, CurrentScene, 0x00A535C4);
    DataPointer(GameOptions*, Options, 0x00AA763C);
    DataPointer(short, CurrentSceneInt, 0x00A535C4);
    DataPointer(Category, CurrentCategory, 0x00A535E0);
    DataPointer(byte, CurrentCategoryInt, 0x00A535E0);

    DataPointer(struct_Timer, Timer, 0x00A535DC);
    DataPointer(GameStates, GameState, 0x00A535E2);
    DataArray(Controller, PlayerControllers, 0x004416D8, 5);
    DataPointer(float, Controller1RStickX, 0x43E5C0);
    DataPointer(float, Controller1RStickY, 0x43E5C4);
    DataPointer(float, Controller1LStickX, 0x43E5C8);
    DataPointer(float, Controller1LStickY, 0x43E5CC);
    static char* CurrentSceneName = (char*)(baseAddress + 0x00A5359C);

    // Palettes
    DataArray(SHORT, Palette0, 0x00945B58, 256);
    DataArray(SHORT, Palette1, 0x00945D58, 256);
    DataArray(SHORT, Palette2, 0x00945F58, 256);
    DataArray(SHORT, Palette3, 0x00946158, 256);
    DataArray(SHORT, Palette4, 0x00946358, 256);
    DataArray(SHORT, Palette5, 0x00946558, 256);
    DataArray(SHORT, Palette6, 0x00946758, 256);
    DataArray(SHORT, Palette7, 0x00946958, 256);

    // Objects
    DataPointer(Obj_Ring*, OBJ_Ring, 0x00AC67E0);
    DataPointer(Obj_Player*, OBJ_Player, 0x00AC6838);
    DataPointer(Obj_Camera*, OBJ_Camera, 0x00AC6AA4);
    DataPointer(Obj_Platform*, OBJ_Platform, 0x00AC6CB4);
    DataPointer(Obj_ForceSpin*, OBJ_ForceSpin, 0x00AC6CB4); // TODO: Check if the address is correct.
    DataPointer(Obj_CollapsingPlatform*, OBJ_CollapsingPlatform, 0x00AC66EC);
    DataPointer(Obj_LRZBreakableWall*, OBJ_LRZBreakableWall, 0x00AC6C80);
    DataPointer(Obj_BreakableWall*, OBJ_BreakableWall, 0x00AC6A34);
    DataPointer(Obj_StarPost*, OBJ_StarPost, 0x00AC672C);
    DataPointer(Obj_Button*, OBJ_Button, 0x00AC68BC);
    DataPointer(Obj_Door*, OBJ_Door, 0x00AC68BC);
    DataPointer(Obj_Spikes*, OBJ_Spikes, 0x00AC67BC);
    DataPointer(Obj_Explosion*, OBJ_Explosion, 0x00AC6D84);
    DataPointer(Obj_ScoreBonus*, OBJ_ScoreBonus, 0x00AC6D80);
    DataPointer(Obj_Animals*, OBJ_Animals, 0x00AC6D78);
    DataPointer(Obj_Spring*, OBJ_Spring, 0x00AC6BD8);
    DataPointer(Obj_ItemBox*, OBJ_ItemBox, 0x00AC6F00);
    DataPointer(Obj_SpecialRing*, OBJ_SpecialRing, 0x00AC686C);
    DataPointer(Obj_PlaneSwitch*, OBJ_PlaneSwitch, 0x00AC6C0C);
    DataPointer(Obj_GHZSetup*, OBJ_GHZSetup, 0x00AC6C0C);
    DataPointer(Obj_LRZ1Setup*, OBJ_LRZ1Setup, 0x00AC6DF4);
    DataPointer(Obj_ZONE*, OBJ_ZONE, 0x00AC690C);
    DataPointer(Obj_FXRuby*, OBJ_FXRuby, 0x00AC6EF0);
    DataPointer(Obj_PauseMenu*, OBJ_PauseMenu, 0x00AC6E90);
    DataPointer(Obj_DebugMode*, OBJ_DebugMode, 0x00AC6930);

#pragma endregion

#pragma region Misc

    // Music struct
    struct struct_0
    {
        int anonymous_0;
        int anonymous_1;
        float volume;
        int anonymous_3;
        int anonymous_4;
        int anonymous_5;
        int anonymous_6;
        int hasLoop;
        __int16 anonymous_8;
        BYTE gap22[1];
        char playStatus;
    };
    DataArray(struct_0, stru_26B818, 0x0026B818, 16);

#pragma endregion

#pragma region Entity

    // A NOTE ON HOW TO GET ATTRIBUTE VALUES
    enum ImportantEntityIDs
    {
        ENTITYSLOT_PLAYER1 = 0,
        ENTITYSLOT_PLAYER2 = 1,
        ENTITYSLOT_PLAYER3 = 1,
        ENTITYSLOT_PLAYER4 = 1,
        ENTITYSLOT_MUSIC = 9,
    };

    struct Entity
    {
#pragma region Data
        ///* 0x00000000 */ int XPos;              // 16 bit bitshift (aka 8 is 0x80000 (or 8 << 16))
        ///* 0x00000004 */ int YPos;              // 16 bit bitshift (aka 8 is 0x80000 (or 8 << 16))
        //
        // +---------------------------------+
        // �                                 �
        // � !! Please stop removing this !! �
        // �                                 �
        // +---------------------------------+
        /* 0x00000000 */ Vector2 Position;
        /* 0x00000008 */ int ScaleX;            // 512-based (512 = 0, 1024 = 2, 256 = 1/2)
        /* 0x0000000C */ int ScaleY;            // 512-based (512 = 0, 1024 = 2, 256 = 1/2)
        /* 0x00000010 */ int XVelocity;         // 16 bit bitshift (aka 8 is 0x80000 (or 8 << 16))
        /* 0x00000014 */ int YVelocity;         // 16 bit bitshift (aka 8 is 0x80000 (or 8 << 16))
        /* 0x00000018 */ int UpdateRangeX;      // How many pixels offscreen to keep the object updating
        /* 0x0000001C */ int UpdateRangeY;      // How many pixels offscreen to keep the object updating
        /* 0x00000020 */ int Angle;
        /* 0x00000024 */ int Alpha;             // Transparency
        /* 0x00000028 */ int Rotation;
        /* 0x0000002C */ int Speed;
        /* 0x00000030 */ DWORD field_30;
        /* 0x00000034 */ WORD field_34;
        /* 0x00000036 */ short ObjectID;
        /* 0x00000038 */ BOOL InBounds;
        /* 0x0000003C */ DWORD field_3C;
        /* 0x00000040 */ BOOL TileCollisions;
        /* 0x00000044 */ BOOL Active;
        /* 0x00000048 */ BOOL Grounded;
        /* 0x0000004C */ ActiveStates Priority; // Active State
        /* 0x0000004D */ BYTE Filter;
        /* 0x0000004E */ BYTE Direction;        // 0-3
        /* 0x0000004F */ BYTE DrawOrder;        // The layer the Sprite Draws on (0-14)
        /* 0x00000050 */ BYTE CollisionLayers;
        /* 0x00000051 */ BYTE CollisionPlane;
        /* 0x00000052 */ CollisionModes CollisionMode;
        /* 0x00000053 */ DrawingFX DrawFX;
        /* 0x00000054 */ InkEffect InkEffect;
        /* 0x00000055 */ BYTE Visible;
        /* 0x00000056 */ BYTE ActiveScreens;
        /* 0x00000055 */ BYTE field_57;


#pragma endregion

        void Move(short x, short y)
        {
            Position.X = x;
            Position.Y = y;
        }
        void AddVelocity(int x, int y)
        {
            XVelocity += x;
            YVelocity += y;
            Speed += x;
        }
        void SetVelocity(int x, int y)
        {
            XVelocity = x;
            YVelocity = y;
            Speed = x;
        }
        void MultiplyVelocity(float x, float y)
        {
            XVelocity = (int)(XVelocity * x);
            YVelocity = (int)(YVelocity * y);
            Speed = (int)(Speed * x);
        }

        bool InRange(int x1, int y1, int x2, int y2)
        {
            bool inRange = false;
            inRange = (Position.X >= x1 && Position.X <= x2) && (Position.Y >= y1 && Position.Y <= y2);
            return inRange;
        }
    };

    // Struct to fill up the base class
    struct EntityBase
    {
        Entity Base;
        byte Extra[0x400];
    };

    struct EntityPlayer : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ DWORD StateDraw; // Name might not be correct
        /* 0x00000060 */ DWORD dword60;
        /* 0x00000064 */ EntityCamera* Camera;
        /* 0x00000068 */ EntityAnimationData Animation;
        /* 0x00000080 */ EntityAnimationData TailAnimation;
        /* 0x00000098 */ DWORD MaxWalkSpeed;
        /* 0x0000009C */ DWORD MaxJogSpeed;
        /* 0x000000A0 */ DWORD MaxRunSpeed;
        /* 0x000000A4 */ DWORD field_A4;
        /* 0x000000A4 */ DWORD TailRotation;
        /* 0x000000A4 */ DWORD TailDirection;
        /* 0x000000B0 */ short SpriteIndex;
        /* 0x000000B2 */ short SpriteIndexTails;
        /* 0x000000B4 */ WORD StoredAnim;
        /* 0x000000B6 */ unsigned short PlayerID;
        /* 0x000000B8 */ Hitbox* Innerbox;
        /* 0x000000BC */ Hitbox* Outerbox;
        /* 0x000000C0 */ Character Character;
        /* 0x000000C4 */ int RingCount;
        /* 0x000000C8 */ int RingsToNextLife;
        /* 0x000000CC */ ShieldType Shield;
        /* 0x000000D0 */ int LifeCount;
        /* 0x000000D4 */ int Score;
        /* 0x000000D8 */ DWORD TargetScore; // Next Score bonus amount
        /* 0x000000DC */ DWORD CombineRing;
        /* 0x000000E0 */ DWORD Timer;
        /* 0x000000E4 */ DWORD OuttaHereTimer;
        /* 0x000000E8 */ DWORD AbilityTimer;
        /* 0x000000EC */ DWORD SpindashRevCharge;
        /* 0x000000F0 */ DWORD GlideTurnCos;
        /* 0x000000F4 */ DWORD AirLeft;
        /* 0x000000F8 */ DWORD Invincibility;
        /* 0x000000FC */ DWORD SpeedShoesTTL;
        /* 0x00000100 */ DWORD InvincibilityFrames;
        /* 0x00000104 */ DWORD field_104; // For when hit
        /* 0x00000108 */ DWORD Skidding;
        /* 0x0000010C */ DWORD Pushing;
        /* 0x00000110 */ BOOL Underwater;
        /* 0x00000114 */ DWORD GroundedStore;
        /* 0x00000118 */ BOOL IsUpSideDown;
        /* 0x0000011C */ DWORD field_11C;
        /* 0x00000120 */ DWORD field_120;
        /* 0x00000124 */ SuperState SuperState;
        /* 0x00000128 */ DWORD SuperSecondTimer;
        /* 0x0000012C */ DWORD SuperBlendAmount;
        /* 0x00000130 */ DWORD SuperBlendDirection;
        /* 0x00000134 */ DWORD InteractStatus; // TODO: Work out all the Statuses
        /* 0x00000138 */ DWORD ScoreBonus;
        /* 0x0000013C */ DWORD CameraOffset;
        /* 0x00000140 */ DWORD dword140;
        /* 0x00000144 */ DWORD dword144;
        /* 0x00000148 */ DWORD TopSpeed;
        /* 0x0000014C */ int Acceleration;
        /* 0x00000150 */ int Decceleration;
        /* 0x00000154 */ int AirAcceleration;
        /* 0x00000158 */ int AirDecceleration;
        /* 0x0000015C */ int SkidSpeed;
        /* 0x00000160 */ int RollingFriction;
        /* 0x00000164 */ int RollingDeceleration;
        /* 0x00000168 */ int GravityStrength;
        /* 0x0000016C */ int GlideSpeedStore;
        /* 0x00000170 */ int JumpStrength;
        /* 0x00000174 */ DWORD JumpCap;
        /* 0x00000178 */ DWORD dword178;
        /* 0x0000017C */ DWORD dword17C;
        /* 0x00000180 */ DWORD dword180;
        /* 0x00000184 */ DWORD dword184;
        /* 0x00000188 */ DWORD dword188;
        /* 0x0000018C */ DWORD dword18C;
        /* 0x00000190 */ DWORD dword190;
        /* 0x00000194 */ BYTE gap194[8];
        /* 0x0000019C */ DWORD dword19C;
        /* 0x000001A0 */ InputStatus InputStatus;
        /* 0x000001A4 */ int ControllerID;
        /* 0x000001A8 */ DWORD dword1A8;
        /* 0x000001AC */ BOOL Up;
        /* 0x000001B0 */ BOOL Down;
        /* 0x000001B4 */ BOOL Left;
        /* 0x000001B8 */ BOOL Right;
        /* 0x000001BC */ BOOL JumpPress;
        /* 0x000001C0 */ BOOL JumpHold;
        /* 0x000001C4 */ int JumpAbility;
        /* 0x000001C8 */ int field_1C8;
        /* 0x000001CC */ Ability Moveset;
        /* 0x000001D0 */ Ability UpAbility;
        /* 0x000001D4 */ DWORD dword1D4;
        /* 0x000001D8 */ DWORD dword1D8;
        /* 0x000001DC */ DWORD dword1DC;
        /* 0x000001E0 */ DWORD dword1E0;
        /* 0x000001E4 */ DWORD dword1E4;
        /* 0x000001E8 */ DWORD KillFlag;
        /* 0x000001EC */ DWORD dword1EC;
        /* 0x000001F0 */ DWORD dword1F0;
        /* 0x000001F4 */ DWORD dword1F4;
        /* 0x000001F8 */ DWORD dword1F8;
        /* 0x000001FC */ DWORD dword1FC;
        /* 0x00000200 */ DWORD dword200;
        /* 0x00000204 */ DWORD dword204;
        /* 0x00000208 */ DWORD dword208;
        /* 0x0000020C */ DWORD dword20C;
        /* 0x00000210 */ DWORD dword210;
        /* 0x00000214 */ DWORD dword214;
        /* 0x00000218 */ DWORD dword218;
        /* 0x0000021C */ DWORD dword21C;

        void Kill()
        {
            KillFlag = 1;
        }

        Controller& GetController()
        {
            if (ControllerID < 0 && ControllerID > 4)
                return PlayerControllers[0];
            return PlayerControllers[ControllerID];
        }
    };
    struct EntityItemBox : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ int Type;
        /* 0x00000060 */ int ContentsXPos;
        /* 0x00000064 */ int ContentsYPos;
        /* 0x00000068 */ DWORD dword68;
        /* 0x0000006C */ DWORD dword6C;
        /* 0x00000070 */ int ContenseSpeed;
        /* 0x00000074 */ Entity* StoredEntity; //Entity To Grant?
        /* 0x00000078 */ int RandomValue;
        /* 0x0000007C */ int IsFalling;
        /* 0x00000080 */ int isContense;
        /* 0x00000084 */ int Hidden;
        /* 0x00000088 */ int PlaneFilter;
        /* 0x0000008C */ int LRZConvPhys; //Lava Reef Convayer Physics 
        /* 0x00000090 */ EntityAnimationData BrokenData;
        /* 0x000000A8 */ EntityAnimationData ContenseData;
        /* 0x000000C0 */ EntityAnimationData OverlyData;
        /* 0x000000D8 */ EntityAnimationData DebrisData;
        /* 0x000000F0 */ int Parent;
    };
    struct EntityPlatform : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ DWORD StateCollide;
        /* 0x00000060 */ DWORD Type;
        /* 0x00000064 */ DWORD AplitudeX;
        /* 0x00000068 */ DWORD AplitudeY;
        /* 0x0000006C */ DWORD Speed;
        /* 0x00000070 */ DWORD HasTension;
        /* 0x00000074 */ byte FrameId;
        /* 0x00000075 */ byte CollisionType;
        /* 0x00000076 */ byte Field_76;
        /* 0x00000077 */ byte Field_77;
        /* 0x00000078 */ DWORD TileOrginX;
        /* 0x0000007C */ DWORD TileOrginY;
        /* 0x00000080 */ DWORD CenterX;
        /* 0x00000084 */ DWORD CenterY;
        /* 0x00000088 */ DWORD DrawX;
        /* 0x0000008C */ DWORD DrawY;
        /* 0x00000090 */ DWORD CollisionXOffset;
        /* 0x00000094 */ DWORD CollisionYOffset;
        /* 0x00000098 */ DWORD Stood;
        /* 0x0000009C */ DWORD CollapseDelay;
        /* 0x000000A0 */ DWORD StoodAngle;
        /* 0x000000A4 */ byte StoodPlayers;
        /* 0x000000A5 */ byte PushPlayersL;
        /* 0x000000A6 */ byte PushPlayersY;
        /* 0x000000A7 */ byte field_A7;
        /* 0x000000A8 */ Hitbox Hitbox;
        /* 0x000000B0 */ EntityAnimationData Animation;
        /* 0x000000C8 */ DWORD ChildCount;
    };
    struct EntityDust : Entity
    {
        /* 0x00000058 */ int State;
        /* 0x0000005C */ DWORD Timer;
        /* 0x00000060 */ DWORD Parent;
        /* 0x00000064 */ EntityAnimationData Animation;
    };
    struct EntityCamera : Entity
    {
        void* State;
        Entity* CameraTarget;
        int ActiveEntity;
        int ClampedShakeX;
        int ClampedShakeY;
        int DestinationOffsetX;
        int DestinationOffsetY;
        int LastXPos;
        int LastYPos;
        int ShakeX;
        int ShakeY;
        int OffsetX;
        int OffsetY;
        int MinFollowDistanceX;
        int GroundCamOffsetY;
        int PlayerInAir;
        int CustomViewHeightHalf;
        int AdjustY;
        int PanProgress;
        int PanSpeed;
        int PanType;
        Vector2 PanToPositionEnd;
        Vector2 PanToPositionSTart;
        int BoundPanningSpeedX;
        int BoundPanningSpeedY;
        int MinX;
        int MaxX;
        int MinY;
        int MaxY;
    };
    struct EntityRing : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x00000058 */ void* StateDraw;
        /* 0x00000060 */ int Type;
        /* 0x00000064 */ DWORD PlaneFilter;
        /* 0x00000068 */ int RingAmount;
        /* 0x0000006C */ DWORD Timer;
        /* 0x00000070 */ DWORD MaxFrameCount;
        /* 0x00000074 */ DWORD SparkleType2;
        /* 0x00000078 */ DWORD StoredPlayer;
        /* 0x0000007C */ DWORD MoveType;
        /* 0x00000080 */ DWORD AmplitudeX;
        /* 0x00000084 */ DWORD AmplitudeY;
        /* 0x00000088 */ DWORD MoveSpeed;
        /* 0x0000008C */ DWORD XOffset;
        /* 0x00000090 */ DWORD YOffset;
        /* 0x00000094 */ EntityAnimationData Animation;

    };
    struct EntityFXFade : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ int Time;
        /* 0x00000060 */ int SpeedIn;
        /* 0x00000064 */ int Wait;
        /* 0x00000068 */ int SpeedOut;
        /* 0x0000006C */ DWORD Color;
        /* 0x00000070 */ bool OneWay;
        /* 0x00000074 */ bool EventOnly;
        /* 0x00000078 */ bool Overhud;
        /* 0x0000007C */ bool FadeOutBlack;
        /* 0x00000080 */ DWORD dword80;
        /* 0x00000084 */ DWORD dword84;
        /* 0x00000088 */ DWORD Speed;
        /* 0x0000008C */ DWORD dword8C;
        /* 0x00000090 */ DWORD dword90;
        /* 0x00000094 */ EntityAnimationData Animation;
    };
    struct EntityFXRuby : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ DWORD gap5C;
        /* 0x00000060 */ DWORD dword60;
        /* 0x00000064 */ DWORD gap64;
        /* 0x00000068 */ DWORD field_68;
        /* 0x0000006C */ int OutterRadius;
        /* 0x00000070 */ int FadeWhite;
        /* 0x00000074 */ int FadeBlack;
        /* 0x00000078 */ DWORD WaitForTrigger;
        /* 0x0000007C */ DWORD field_7C;
        /* 0x00000080 */ DWORD dword80;
        /* 0x00000084 */ DWORD field_84;
        /* 0x00000088 */ DWORD field_88;
        /* 0x0000008C */ DWORD field_8C;
        /* 0x00000090 */ DWORD field_90;

    };
    struct EntityPauseMenu : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x00000058 */ void* StateDraw;
        /* 0x00000060 */ DWORD field_60;
        /* 0x00000064 */ DWORD field_64;
        /* 0x00000068 */ DWORD field_68;
        /* 0x0000006C */ DWORD field_6C;
        /* 0x00000070 */ DWORD field_70;
        /* 0x00000074 */ DWORD field_74;
        /* 0x00000078 */ DWORD field_78;
        /* 0x0000007C */ short field_7C;
        /* 0x0000007E */ short field_7E;
        /* 0x00000080 */ DWORD field_80;
        /* 0x00000084 */ DWORD field_84;
        /* 0x00000088 */ DWORD field_88;
        /* 0x0000008C */ DWORD field_8C;
        /* 0x00000090 */ DWORD field_90;
        /* 0x00000094 */ DWORD field_94;
        /* 0x00000098 */ DWORD field_98;
        /* 0x0000009C */ DWORD field_9C;
        /* 0x000000A0 */ DWORD field_A0;
        /* 0x000000A4 */ DWORD field_A4;
        /* 0x000000A8 */ DWORD field_A8;
        /* 0x000000AC */ DWORD field_AC;
        /* 0x000000B0 */ DWORD field_B0;
        /* 0x000000B4 */ EntityAnimationData Animation;
        /* 0x000000CC */ DWORD field_CC;
        /* 0x000000D0 */ DWORD field_D0;
        /* 0x000000D4 */ DWORD field_D4;

    };
    struct EntityUIVsZoneButton : Entity
    {
        /* 0x00000058 */ BYTE gap[164];
        /* 0x000000FC */ DWORD Filter; //Might not Exist 
        /* 0x00000100 */ bool Disabled;
        /* 0x00000104 */ DWORD ZoneID;
        /* 0x00000108 */ DWORD Act;
        /* 0x0000010C */ bool XOut;
        /* 0x00000110 */ bool Obfuscate;
    };
    struct EntityUIText : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ void* StateDraw;
        /* 0x00000060 */ wchar_t* Text;
        /* 0x00000064 */ WORD TextLength;
        /* 0x00000066 */ WORD field_66;
        /* 0x00000068 */ DWORD Tag;
        /* 0x0000006C */ DWORD field_6C;
        /* 0x00000070 */ DWORD Selectable;
        /* 0x00000074 */ WORD AnimListID;
        /* 0x00000076 */ WORD gap76;
        /* 0x00000078 */ DWORD Align;
        /* 0x0000007C */ DWORD Data0;
        /* 0x00000080 */ DWORD Data1;
        /* 0x00000084 */ DWORD Data2;
        /* 0x00000088 */ DWORD  Data3;
        /* 0x0000008C */ DWORD Highlighted;
        /* 0x00000090 */ SonicMania::EntityAnimationData  AnimData;
    };
    struct EntityMotoBug : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ DWORD dword5C;
        /* 0x00000060 */ DWORD Timer;
        /* 0x00000064 */ DWORD StartX;
        /* 0x00000068 */ DWORD StartY;
        /* 0x0000006C */ BYTE StartDirection;
        /* 0x0000006D */ BYTE field_6D;
        /* 0x0000006E */ BYTE field_6E;
        /* 0x0000006F */ BYTE field_6F;
        /* 0x00000070 */ DWORD dword70;
        /* 0x00000074 */ EntityAnimationData Animation;
        /* 0x00000088 */ DWORD dword88;
        /* 0x0000008C */ DWORD dword8C;
        /* 0x00000090 */ DWORD dword90;

    };
    struct EntityClapperboard : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ DWORD dword5C;
        /* 0x00000060 */ BYTE TakeID;
        /* 0x00000061 */ BYTE Field_61;
        /* 0x00000062 */ BYTE Field_62;
        /* 0x00000063 */ BYTE Field_63;
        /* 0x00000064 */ DWORD dword64;
        /* 0x00000068 */ DWORD StarField_68;
        /* 0x0000006C */ Hitbox Hotbox;
        /* 0x00000074 */ DWORD dword74;
        /* 0x00000078 */ DWORD dword78;
        /* 0x0000007C */ DWORD dword7C;
        /* 0x00000080 */ DWORD dword80;
        /* 0x00000084 */ EntityAnimationData Animation84;
        /* 0x0000009C */ EntityAnimationData Animation9C;
        /* 0x000000B4 */ EntityAnimationData AnimationB4;
        /* 0x000000CC */ EntityAnimationData AnimationCC;
    };
    struct EntityHUD : Entity
    {
        /* 0x00000058 */ DWORD dword58;
        /* 0x0000005C */ DWORD dword5C;
        /* 0x00000060 */ DWORD dword60;
        /* 0x00000064 */ DWORD dword64;
        /* 0x00000068 */ DWORD dword68;
        /* 0x0000006C */ DWORD dword6C;
        /* 0x00000070 */ DWORD dword70;
        /* 0x00000074 */ DWORD dword74;
        /* 0x00000078 */ DWORD dword78;
        /* 0x0000007C */ DWORD dword7C;
        /* 0x00000080 */ DWORD dword80;
        /* 0x00000084 */ DWORD dword84;
        /* 0x00000088 */ DWORD dword88;
        /* 0x0000008C */ DWORD dword8C;
        /* 0x00000090 */ DWORD dword90;
        /* 0x00000094 */ DWORD dword94;
        /* 0x00000098 */ DWORD dword98;
        /* 0x0000009C */ DWORD dword9C;
        /* 0x000000A0 */ DWORD dwordA0;
        /* 0x000000A4 */ DWORD CompetitionSomething;
        /* 0x000000A8 */ DWORD dwordA8;
        /* 0x000000AC */ DWORD dwordAC;
        /* 0x000000B0 */ DWORD dwordB0;
        /* 0x000000B4 */ DWORD dwordB4;
        /* 0x000000B8 */ DWORD dwordB8;
        /* 0x000000BC */ DWORD dwordBC;



    };
    struct EntitySpring : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ int Type;
        /* 0x00000060 */ int FlipFlag;
        /* 0x00000064 */ BYTE PlaneFilter;
        /* 0x00000065 */ BYTE gap65;
        /* 0x00000066 */ BYTE field_66;
        /* 0x00000067 */ BYTE field_67;
        /* 0x00000068 */ DWORD field_68;
        /* 0x0000006C */ EntityAnimationData Animation;
        /* 0x00000084 */ DWORD dword84;
        /* 0x00000088 */ DWORD dword88;
    };
    struct EntityUFOSphere : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ int Type;
        /* 0x00000060 */ int Behavior;
        /* 0x00000064 */ int Height;
        /* 0x00000068 */ int Speed;
        /* 0x0000006C */ int MatAngleX;
        /* 0x00000070 */ int MatAngleY;
        /* 0x00000074 */ int AmplitudeX;
        /* 0x00000078 */ int AmplitudeY;
        /* 0x0000007C */ DWORD field7C;
        /* 0x00000080 */ DWORD dword80;
        /* 0x00000084 */ DWORD dword84;
        /* 0x00000088 */ DWORD dword88;
        /* 0x0000008C */ DWORD dword8C;
        /* 0x00000090 */ DWORD dword90;
        /* 0x00000094 */ DWORD dword94;
        /* 0x00000098 */ DWORD dword98;
        /* 0x0000009C */ DWORD dword9C;
        /* 0x000000A0 */ BYTE gapC0[148];
        /* 0x00000140 */ EntityAnimationData Animation;
    };
    struct EntityUFOPlayer : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x00000058 */ int MachQuota1;
        /* 0x00000060 */ int MachQuota4;
        /* 0x00000064 */ int MachQuota3;
        /* 0x00000068 */ int StartingRings;
        /* 0x0000006C */ DWORD field_6C;
        /* 0x00000070 */ DWORD field_70;
        /* 0x00000074 */ DWORD field_74;
        /* 0x00000078 */ DWORD field_78;
        /* 0x0000007C */ DWORD field_7C;
        /* 0x00000080 */ DWORD field_80;
        /* 0x00000084 */ DWORD field_84;
        /* 0x00000088 */ int BumperSFXTimer;
        /* 0x0000008C */ DWORD field_8C;
        /* 0x00000090 */ DWORD field_90;
        /* 0x00000094 */ DWORD field_94;
        /* 0x00000098 */ DWORD field_98;
        /* 0x0000009C */ DWORD field_9C;
        /* 0x000000A0 */ DWORD field_A0;
        /* 0x000000A4 */ DWORD field_A4;
        /* 0x000000A8 */ DWORD field_A8;
        /* 0x000000AC */ DWORD field_AC;
        /* 0x000000B0 */ DWORD field_B0;
        /* 0x000000B4 */ DWORD field_B4;
        /* 0x000000B8 */ DWORD field_B8;
        /* 0x000000BC */ DWORD field_BC;
        /* 0x000000C0 */ DWORD field_C0;
        /* 0x000000C4 */ DWORD field_C4;
        /* 0x000000C8 */ DWORD field_C8;
        /* 0x000000CC */ DWORD field_CC;
        /* 0x000000D0 */ DWORD field_D0;
        /* 0x000000D4 */ DWORD field_D4;
        /* 0x000000D8 */ DWORD field_D8;
        /* 0x000000DC */ DWORD field_DC;
        /* 0x000000E0 */ DWORD field_E0;
        /* 0x000000E4 */ DWORD field_E4;
        /* 0x000000E8 */ DWORD field_E8;
        /* 0x000000EC */ DWORD field_EC;
        /* 0x000000F0 */ DWORD field_F0;
        /* 0x000000F4 */ DWORD field_F4;
        /* 0x000000F8 */ DWORD field_F8;
        /* 0x000000FC */ DWORD field_FC;
        /* 0x00000100 */ DWORD field_100;
        /* 0x00000104 */ DWORD field_104;
        /* 0x00000108 */ DWORD field_108;
        /* 0x0000010C */ DWORD field_10C;
        /* 0x00000110 */ DWORD field_110;
        /* 0x00000114 */ DWORD field_114;
        /* 0x00000118 */ DWORD field_118;
        /* 0x0000011C */ DWORD field_11C;
        /* 0x00000120 */ DWORD field_120;
        /* 0x00000124 */ DWORD field_124;
        /* 0x00000128 */ DWORD field_128;
        /* 0x0000012C */ DWORD field_12C;
        /* 0x00000130 */ DWORD field_130;
        /* 0x00000134 */ DWORD field_134;
        /* 0x00000138 */ DWORD field_138;
        /* 0x0000013C */ DWORD field_13C;
        /* 0x00000140 */ DWORD field_140;
        /* 0x00000144 */ DWORD field_144;
        /* 0x00000148 */ DWORD field_148;
        /* 0x0000014C */ DWORD field_14C;
        /* 0x00000150 */ DWORD field_150;
        /* 0x00000154 */ DWORD field_154;
        /* 0x00000158 */ DWORD field_158;
        /* 0x0000015C */ DWORD field_15C;
        /* 0x00000160 */ DWORD field_160;
        /* 0x00000164 */ DWORD field_164;
        /* 0x00000168 */ DWORD field_168;
        /* 0x0000016C */ DWORD field_16C;
        /* 0x00000170 */ DWORD field_170;
        /* 0x00000174 */ DWORD field_174;
        /* 0x00000178 */ DWORD field_178;
        /* 0x0000017C */ DWORD field_17C;
        /* 0x00000180 */ DWORD field_180;
        /* 0x00000184 */ DWORD field_184;
        /* 0x00000188 */ DWORD field_188;
        /* 0x0000018C */ DWORD field_18C;
        /* 0x00000190 */ DWORD field_190;
        /* 0x00000194 */ DWORD field_194;
        /* 0x00000198 */ DWORD field_198;
        /* 0x0000019C */ InputStatus InputState;
        /* 0x000001A0 */ DWORD field_1A0;
        /* 0x000001A4 */ BOOL Up;
        /* 0x000001A8 */ BOOL Down;
        /* 0x000001AC */ BOOL Left;
        /* 0x000001B0 */ BOOL Right;
        /* 0x000001B4 */ BOOL JumpPress;
        /* 0x000001B8 */ BOOL JumpHold;
        /* 0x000001BC */ EntityAnimationData Animation;
    };
    struct EntityTitleCard : Entity
    {
        /* 00000058 */ void* State;
        /* 0000005C */ void* StateDraw;
        /* 00000060 */ DWORD gap60;
        /* 00000064 */ DWORD field_64;
        /* 00000068 */ DWORD dword68;
        /* 0000006C */ DWORD dword6C;
        /* 00000070 */ DWORD dword70;
        /* 00000074 */ DWORD dword74;
        /* 00000078 */ DWORD dword78;
        /* 0000007C */ DWORD dword7C;
        /* 00000080 */ DWORD gap80;
        /* 00000084 */ DWORD field_84;
        /* 00000088 */ DWORD field_88;
        /* 0000008C */ DWORD field_8C;
        /* 00000090 */ DWORD field_90;
        /* 00000094 */ DWORD field_94;
        /* 00000098 */ DWORD field_98;
        /* 0000009C */ DWORD field_9C;
        /* 000000A0 */ Vector2 Points1;
        /* 000000C0 */ Vector2 Points2;
        /* 000000E0 */ Vector2 Points3;
        /* 00000100 */ DWORD field_100;
        /* 00000104 */ DWORD field_104;
        /* 00000108 */ DWORD field_108;
        /* 0000010C */ DWORD field_10C;
        /* 00000110 */ DWORD field_110;
        /* 00000114 */ DWORD field_114;
        /* 00000118 */ DWORD field_118;
        /* 0000011C */ BYTE field_11C;
        /* 0000011D */ BYTE field_11D;
        /* 0000011E */ BYTE field_11E;
        /* 0000011F */ BYTE field_11F;
        /* 00000120 */ BYTE field_120;
        /* 00000121 */ BYTE field_121;
        /* 00000122 */ BYTE field_122;
        /* 00000123 */ BYTE field_123;
        /* 00000124 */ BYTE field_124;
        /* 00000125 */ BYTE field_125;
        /* 00000126 */ BYTE field_126;
        /* 00000127 */ BYTE field_127;
        /* 00000128 */ BYTE field_128;
        /* 00000129 */ BYTE field_129;
        /* 0000012A */ BYTE field_12A;
        /* 0000012B */ BYTE field_12B;
        /* 0000012C */ BYTE field_12C;
        /* 0000012D */ BYTE field_12D;
        /* 0000012E */ BYTE field_12E;
        /* 0000012F */ BYTE field_12F;
        /* 00000130 */ BYTE field_130;
        /* 00000131 */ BYTE field_131;
        /* 00000132 */ BYTE field_132;
        /* 00000133 */ BYTE field_133;
        /* 00000134 */ BYTE field_134;
        /* 00000135 */ BYTE field_135;
        /* 00000136 */ BYTE field_136;
        /* 00000137 */ BYTE field_137;
        /* 00000138 */ BYTE field_138;
        /* 00000139 */ BYTE field_139;
        /* 0000013A */ BYTE field_13A;
        /* 0000013B */ BYTE field_13B;
        /* 0000013C */ BYTE field_13C;
        /* 0000013D */ BYTE field_13D;
        /* 0000013E */ BYTE field_13E;
        /* 0000013F */ BYTE field_13F;
        /* 00000140 */ BYTE field_140;
        /* 00000141 */ BYTE field_141;
        /* 00000142 */ BYTE field_142;
        /* 00000143 */ BYTE field_143;
        /* 00000144 */ BYTE field_144;
        /* 00000145 */ BYTE field_145;
        /* 00000146 */ BYTE field_146;
        /* 00000147 */ BYTE field_147;
        /* 00000148 */ BYTE field_148;
        /* 00000149 */ BYTE field_149;
        /* 0000014A */ BYTE field_14A;
        /* 0000014B */ BYTE field_14B;
        /* 0000014C */ BYTE field_14C;
        /* 0000014D */ BYTE field_14D;
        /* 0000014E */ BYTE field_14E;
        /* 0000014F */ BYTE field_14F;
        /* 00000150 */ BYTE field_150;
        /* 00000151 */ BYTE field_151;
        /* 00000152 */ BYTE field_152;
        /* 00000153 */ BYTE field_153;
        /* 00000154 */ BYTE field_154;
        /* 00000155 */ BYTE field_155;
        /* 00000156 */ BYTE field_156;
        /* 00000157 */ BYTE field_157;
        /* 00000158 */ BYTE field_158;
        /* 00000159 */ BYTE field_159;
        /* 0000015A */ BYTE field_15A;
        /* 0000015B */ BYTE field_15B;
        /* 0000015C */ BYTE field_15C;
        /* 0000015D */ BYTE field_15D;
        /* 0000015E */ BYTE field_15E;
        /* 0000015F */ BYTE field_15F;
        /* 00000160 */ BYTE field_160;
        /* 00000161 */ BYTE field_161;
        /* 00000162 */ BYTE field_162;
        /* 00000163 */ BYTE field_163;
        /* 00000164 */ BYTE field_164;
        /* 00000165 */ BYTE field_165;
        /* 00000166 */ BYTE field_166;
        /* 00000167 */ BYTE field_167;
        /* 00000168 */ BYTE field_168;
        /* 00000169 */ BYTE field_169;
        /* 0000016A */ BYTE field_16A;
        /* 0000016B */ BYTE field_16B;
        /* 0000016C */ BYTE field_16C;
        /* 0000016D */ BYTE field_16D;
        /* 0000016E */ BYTE field_16E;
        /* 0000016F */ BYTE field_16F;
        /* 00000170 */ BYTE field_170;
        /* 00000171 */ BYTE field_171;
        /* 00000172 */ BYTE field_172;
        /* 00000173 */ BYTE field_173;
        /* 00000174 */ BYTE field_174;
        /* 00000175 */ BYTE field_175;
        /* 00000176 */ BYTE field_176;
        /* 00000177 */ BYTE field_177;
        /* 00000178 */ BYTE field_178;
        /* 00000179 */ BYTE field_179;
        /* 0000017A */ BYTE field_17A;
        /* 0000017B */ BYTE field_17B;
        /* 0000017C */ BYTE field_17C;
        /* 0000017D */ BYTE field_17D;
        /* 0000017E */ BYTE field_17E;
        /* 0000017F */ BYTE field_17F;
        /* 00000180 */ BYTE field_180;
        /* 00000181 */ BYTE field_181;
        /* 00000182 */ BYTE field_182;
        /* 00000183 */ BYTE field_183;
        /* 00000184 */ BYTE field_184;
        /* 00000185 */ BYTE field_185;
        /* 00000186 */ BYTE field_186;
        /* 00000187 */ BYTE field_187;
        /* 00000188 */ BYTE field_188;
        /* 00000189 */ BYTE field_189;
        /* 0000018A */ BYTE field_18A;
        /* 0000018B */ BYTE field_18B;
        /* 0000018C */ BYTE field_18C;
        /* 0000018D */ BYTE field_18D;
        /* 0000018E */ BYTE field_18E;
        /* 0000018F */ BYTE field_18F;
        /* 00000190 */ BYTE field_190;
        /* 00000191 */ BYTE field_191;
        /* 00000192 */ BYTE field_192;
        /* 00000193 */ BYTE field_193;
        /* 00000194 */ BYTE field_194;
        /* 00000195 */ BYTE field_195;
        /* 00000196 */ BYTE field_196;
        /* 00000197 */ BYTE field_197;
        /* 00000198 */ BYTE field_198;
        /* 00000199 */ BYTE field_199;
        /* 0000019A */ BYTE field_19A;
        /* 0000019B */ BYTE field_19B;
        /* 0000019C */ BYTE field_19C;
        /* 0000019D */ BYTE field_19D;
        /* 0000019E */ BYTE field_19E;
        /* 0000019F */ BYTE field_19F;
        /* 000001A0 */ BYTE field_1A0;
        /* 000001A1 */ BYTE field_1A1;
        /* 000001A2 */ BYTE field_1A2;
        /* 000001A3 */ BYTE field_1A3;
        /* 000001A4 */ BYTE field_1A4;
        /* 000001A5 */ BYTE field_1A5;
        /* 000001A6 */ BYTE field_1A6;
        /* 000001A7 */ BYTE field_1A7;
        /* 000001A8 */ BYTE field_1A8;
        /* 000001A9 */ BYTE field_1A9;
        /* 000001AA */ BYTE field_1AA;
        /* 000001AB */ BYTE field_1AB;
        /* 000001AC */ BYTE field_1AC;
        /* 000001AD */ BYTE field_1AD;
        /* 000001AE */ BYTE field_1AE;
        /* 000001AF */ BYTE field_1AF;
        /* 000001B0 */ BYTE field_1B0;
        /* 000001B1 */ BYTE field_1B1;
        /* 000001B2 */ BYTE field_1B2;
        /* 000001B3 */ BYTE field_1B3;
        /* 000001B4 */ BYTE field_1B4;
        /* 000001B5 */ BYTE field_1B5;
        /* 000001B6 */ BYTE field_1B6;
        /* 000001B7 */ BYTE field_1B7;
        /* 000001B8 */ BYTE field_1B8;
        /* 000001B9 */ BYTE field_1B9;
        /* 000001BA */ BYTE field_1BA;
        /* 000001BB */ BYTE field_1BB;
        /* 000001BC */ BYTE field_1BC;
        /* 000001BD */ BYTE field_1BD;
        /* 000001BE */ BYTE field_1BE;
        /* 000001BF */ BYTE field_1BF;
        /* 000001C0 */ WCHAR* ZoneName;
        /* 000001C4 */ BYTE field_1C4;
        /* 000001C5 */ BYTE field_1C5;
        /* 000001C6 */ BYTE field_1C6;
        /* 000001C7 */ BYTE field_1C7;
        /* 000001C8 */ BYTE field_1C8;
        /* 000001C9 */ BYTE field_1C9;
        /* 000001CA */ BYTE field_1CA;
        /* 000001CB */ BYTE field_1CB;
        /* 000001CC */ BYTE field_1CC;
        /* 000001CD */ BYTE field_1CD;
        /* 000001CE */ BYTE field_1CE;
        /* 000001CF */ BYTE field_1CF;
        /* 000001D0 */ BYTE field_1D0;
        /* 000001D1 */ BYTE field_1D1;
        /* 000001D2 */ BYTE field_1D2;
        /* 000001D3 */ BYTE field_1D3;
        /* 000001D4 */ BYTE field_1D4;
        /* 000001D5 */ BYTE field_1D5;
        /* 000001D6 */ BYTE field_1D6;
        /* 000001D7 */ BYTE field_1D7;
        /* 000001D8 */ BYTE field_1D8;
        /* 000001D9 */ BYTE field_1D9;
        /* 000001DA */ BYTE field_1DA;
        /* 000001DB */ BYTE field_1DB;
        /* 000001DC */ BYTE field_1DC;
        /* 000001DD */ BYTE field_1DD;
        /* 000001DE */ BYTE field_1DE;
        /* 000001DF */ BYTE field_1DF;
        /* 000001E0 */ BYTE field_1E0;
        /* 000001E1 */ BYTE field_1E1;
        /* 000001E2 */ BYTE field_1E2;
        /* 000001E3 */ BYTE field_1E3;
        /* 000001E4 */ BYTE field_1E4;
        /* 000001E5 */ BYTE field_1E5;
        /* 000001E6 */ BYTE field_1E6;
        /* 000001E7 */ BYTE field_1E7;
        /* 000001E8 */ DWORD dword1E8;
        /* 000001EC */ DWORD gap1EC;
        /* 000001F0 */ BYTE field_1F0;
        /* 000001F1 */ BYTE field_1F1;
        /* 000001F2 */ BYTE field_1F2;
        /* 000001F3 */ BYTE field_1F3;
        /* 000001F4 */ BYTE field_1F4;
        /* 000001F5 */ BYTE field_1F5;
        /* 000001F6 */ BYTE field_1F6;
        /* 000001F7 */ BYTE field_1F7;
        /* 000001F8 */ BYTE field_1F8;
        /* 000001F9 */ BYTE field_1F9;
        /* 000001FA */ BYTE field_1FA;
        /* 000001FB */ BYTE field_1FB;
        /* 000001FC */ BYTE field_1FC;
        /* 000001FD */ BYTE field_1FD;
        /* 000001FE */ BYTE field_1FE;
        /* 000001FF */ BYTE field_1FF;
        /* 00000200 */ BYTE field_200;
        /* 00000201 */ BYTE field_201;
        /* 00000202 */ BYTE field_202;
        /* 00000203 */ BYTE field_203;
        /* 00000204 */ BYTE field_204;
        /* 00000205 */ BYTE field_205;
        /* 00000206 */ BYTE field_206;
        /* 00000207 */ BYTE field_207;
        /* 00000208 */ BYTE field_208;
        /* 00000209 */ BYTE field_209;
        /* 0000020A */ BYTE field_20A;
        /* 0000020B */ BYTE field_20B;
        /* 0000020C */ BYTE field_20C;
        /* 0000020D */ BYTE field_20D;
        /* 0000020E */ BYTE field_20E;
        /* 0000020F */ BYTE field_20F;
        /* 00000210 */ BYTE field_210;
        /* 00000211 */ BYTE field_211;
        /* 00000212 */ BYTE field_212;
        /* 00000213 */ BYTE field_213;
        /* 00000214 */ BYTE field_214;
        /* 00000215 */ BYTE field_215;
        /* 00000216 */ BYTE field_216;
        /* 00000217 */ BYTE field_217;
        /* 00000218 */ BYTE field_218;
        /* 00000219 */ BYTE field_219;
        /* 0000021A */ BYTE field_21A;
        /* 0000021B */ BYTE field_21B;
        /* 0000021C */ BYTE field_21C;
        /* 0000021D */ BYTE field_21D;
        /* 0000021E */ BYTE field_21E;
        /* 0000021F */ BYTE field_21F;
        /* 00000220 */ BYTE field_220;
        /* 00000221 */ BYTE field_221;
        /* 00000222 */ BYTE field_222;
        /* 00000223 */ BYTE field_223;
        /* 00000224 */ BYTE field_224;
        /* 00000225 */ BYTE field_225;
        /* 00000226 */ BYTE field_226;
        /* 00000227 */ BYTE field_227;
        /* 00000228 */ BYTE field_228;
        /* 00000229 */ BYTE field_229;
        /* 0000022A */ BYTE field_22A;
        /* 0000022B */ BYTE field_22B;
        /* 0000022C */ BYTE field_22C;
        /* 0000022D */ BYTE field_22D;
        /* 0000022E */ BYTE field_22E;
        /* 0000022F */ BYTE field_22F;
        /* 00000230 */ BYTE field_230;
        /* 00000231 */ BYTE field_231;
        /* 00000232 */ BYTE field_232;
        /* 00000233 */ BYTE field_233;
        /* 00000234 */ BYTE field_234;
        /* 00000235 */ BYTE field_235;
        /* 00000236 */ BYTE field_236;
        /* 00000237 */ BYTE field_237;
        /* 00000238 */ BYTE field_238;
        /* 00000239 */ BYTE field_239;
        /* 0000023A */ BYTE field_23A;
        /* 0000023B */ BYTE field_23B;
        /* 0000023C */ BYTE field_23C;
        /* 0000023D */ BYTE field_23D;
        /* 0000023E */ BYTE field_23E;
        /* 0000023F */ BYTE field_23F;
        /* 00000240 */ BYTE field_240;
        /* 00000241 */ BYTE field_241;
        /* 00000242 */ BYTE field_242;
        /* 00000243 */ BYTE field_243;
        /* 00000244 */ BYTE field_244;
        /* 00000245 */ BYTE field_245;
        /* 00000246 */ BYTE field_246;
        /* 00000247 */ BYTE field_247;
        /* 00000248 */ BYTE field_248;
        /* 00000249 */ BYTE field_249;
        /* 0000024A */ BYTE field_24A;
        /* 0000024B */ BYTE field_24B;
        /* 0000024C */ BYTE field_24C;
        /* 0000024D */ BYTE field_24D;
        /* 0000024E */ BYTE field_24E;
        /* 0000024F */ BYTE field_24F;
        /* 00000250 */ BYTE field_250;
        /* 00000251 */ BYTE field_251;
        /* 00000252 */ BYTE field_252;
        /* 00000253 */ BYTE field_253;
        /* 00000254 */ BYTE field_254;
        /* 00000255 */ BYTE field_255;
        /* 00000256 */ BYTE field_256;
        /* 00000257 */ BYTE field_257;
        /* 00000258 */ BYTE field_258;
        /* 00000259 */ BYTE field_259;
        /* 0000025A */ BYTE field_25A;
        /* 0000025B */ BYTE field_25B;
        /* 0000025C */ BYTE field_25C;
        /* 0000025D */ BYTE field_25D;
        /* 0000025E */ BYTE field_25E;
        /* 0000025F */ BYTE field_25F;
        /* 00000260 */ BYTE field_260;
        /* 00000261 */ BYTE field_261;
        /* 00000262 */ BYTE field_262;
        /* 00000263 */ BYTE field_263;
        /* 00000264 */ BYTE field_264;
        /* 00000265 */ BYTE field_265;
        /* 00000266 */ BYTE field_266;
        /* 00000267 */ BYTE field_267;
        /* 00000268 */ BYTE field_268;
        /* 00000269 */ BYTE field_269;
        /* 0000026A */ BYTE field_26A;
        /* 0000026B */ BYTE field_26B;
        /* 0000026C */ BYTE field_26C;
        /* 0000026D */ BYTE field_26D;
        /* 0000026E */ BYTE field_26E;
        /* 0000026F */ BYTE field_26F;
        /* 00000270 */ BYTE field_270;
        /* 00000271 */ BYTE field_271;
        /* 00000272 */ BYTE field_272;
        /* 00000273 */ BYTE field_273;
        /* 00000274 */ BYTE field_274;
        /* 00000275 */ BYTE field_275;
        /* 00000276 */ BYTE field_276;
        /* 00000277 */ BYTE field_277;
        /* 00000278 */ BYTE field_278;
        /* 00000279 */ BYTE field_279;
        /* 0000027A */ BYTE field_27A;
        /* 0000027B */ BYTE field_27B;
        /* 0000027C */ BYTE field_27C;
        /* 0000027D */ BYTE field_27D;
        /* 0000027E */ BYTE field_27E;
        /* 0000027F */ BYTE field_27F;
        /* 00000280 */ BYTE field_280;
        /* 00000281 */ BYTE field_281;
        /* 00000282 */ BYTE field_282;
        /* 00000283 */ BYTE field_283;
        /* 00000284 */ BYTE field_284;
        /* 00000285 */ BYTE field_285;
        /* 00000286 */ BYTE field_286;
        /* 00000287 */ BYTE field_287;
        /* 00000288 */ BYTE field_288;
        /* 00000289 */ BYTE field_289;
        /* 0000028A */ BYTE field_28A;
        /* 0000028B */ BYTE field_28B;
        /* 0000028C */ BYTE field_28C;
        /* 0000028D */ BYTE field_28D;
        /* 0000028E */ BYTE field_28E;
        /* 0000028F */ BYTE field_28F;
        /* 00000290 */ BYTE field_290;
        /* 00000291 */ BYTE field_291;
        /* 00000292 */ BYTE field_292;
        /* 00000293 */ BYTE field_293;
        /* 00000294 */ BYTE field_294;
        /* 00000295 */ BYTE field_295;
        /* 00000296 */ BYTE field_296;
        /* 00000297 */ BYTE field_297;
        /* 00000298 */ BYTE field_298;
        /* 00000299 */ BYTE field_299;
        /* 0000029A */ BYTE field_29A;
        /* 0000029B */ BYTE field_29B;
        /* 0000029C */ BYTE field_29C;
        /* 0000029D */ BYTE field_29D;
        /* 0000029E */ BYTE field_29E;
        /* 0000029F */ BYTE field_29F;
        /* 000002A0 */ BYTE field_2A0;
        /* 000002A1 */ BYTE field_2A1;
        /* 000002A2 */ BYTE field_2A2;
        /* 000002A3 */ BYTE field_2A3;
        /* 000002A4 */ BYTE field_2A4;
        /* 000002A5 */ BYTE field_2A5;
        /* 000002A6 */ BYTE field_2A6;
        /* 000002A7 */ BYTE field_2A7;
        /* 000002A8 */ BYTE field_2A8;
        /* 000002A9 */ BYTE field_2A9;
        /* 000002AA */ BYTE field_2AA;
        /* 000002AB */ BYTE field_2AB;
        /* 000002AC */ BYTE field_2AC;
        /* 000002AD */ BYTE field_2AD;
        /* 000002AE */ BYTE field_2AE;
        /* 000002AF */ BYTE field_2AF;
        /* 000002B0 */ BYTE field_2B0;
        /* 000002B1 */ BYTE field_2B1;
        /* 000002B2 */ BYTE field_2B2;
        /* 000002B3 */ BYTE field_2B3;
        /* 000002B4 */ BYTE field_2B4;
        /* 000002B5 */ BYTE field_2B5;
        /* 000002B6 */ BYTE field_2B6;
        /* 000002B7 */ BYTE field_2B7;
        /* 000002B8 */ BYTE field_2B8;
        /* 000002B9 */ BYTE field_2B9;
        /* 000002BA */ BYTE field_2BA;
        /* 000002BB */ BYTE field_2BB;
        /* 000002BC */ BYTE field_2BC;
        /* 000002BD */ BYTE field_2BD;
        /* 000002BE */ BYTE field_2BE;
        /* 000002BF */ BYTE field_2BF;
        /* 000002C0 */ BYTE field_2C0;
        /* 000002C1 */ BYTE field_2C1;
        /* 000002C2 */ BYTE field_2C2;
        /* 000002C3 */ BYTE field_2C3;
        /* 000002C4 */ BYTE field_2C4;
        /* 000002C5 */ BYTE field_2C5;
        /* 000002C6 */ BYTE field_2C6;
        /* 000002C7 */ BYTE field_2C7;
        /* 000002C8 */ BYTE field_2C8;
        /* 000002C9 */ BYTE field_2C9;
        /* 000002CA */ BYTE field_2CA;
        /* 000002CB */ BYTE field_2CB;
        /* 000002CC */ BYTE field_2CC;
        /* 000002CD */ BYTE field_2CD;
        /* 000002CE */ BYTE field_2CE;
        /* 000002CF */ BYTE field_2CF;
        /* 000002D0 */ BYTE field_2D0;
        /* 000002D1 */ BYTE field_2D1;
        /* 000002D2 */ BYTE field_2D2;
        /* 000002D3 */ BYTE field_2D3;
        /* 000002D4 */ BYTE field_2D4;
        /* 000002D5 */ BYTE field_2D5;
        /* 000002D6 */ BYTE field_2D6;
        /* 000002D7 */ BYTE field_2D7;
        /* 000002D8 */ BYTE field_2D8;
        /* 000002D9 */ BYTE field_2D9;
        /* 000002DA */ BYTE field_2DA;
        /* 000002DB */ BYTE field_2DB;
        /* 000002DC */ DWORD field_2DC;
        /* 000002E0 */ DWORD field_2E0;
        /* 000002E4 */ DWORD dword2E4;
        /* 000002E8 */ DWORD dword2E8;
        /* 000002EC */ DWORD dword2EC;
        /* 000002F0 */ BYTE ActID;
        /* 000002F2 */ BYTE field_2F2;
        /* 000002F1 */ BYTE field_2F1;
        /* 000002F3 */ BYTE field_2F3;
        /* 000002F4 */ DWORD dword2F4;
        /* 000002F8 */ DWORD dword2F8;
        /* 000002FC */ DWORD dword2FC;
        /* 00000300 */ DWORD dword300;
        /* 00000304 */ EntityAnimationData DecorationData;
        /* 0000031C */ EntityAnimationData NameLetterData;
        /* 00000334 */ EntityAnimationData ZoneLetterData;
        /* 0000034C */ EntityAnimationData ActNumbersData;
    };
    struct EntityLoveTester : Entity
    {
        /* 00000058 */ void* State;
        /* 0000005C */ DWORD field_5C;
        /* 00000060 */ BYTE field_60;
        /* 00000061 */ BYTE field_61;
        /* 00000062 */ BYTE field_62;
        /* 00000063 */ BYTE field_63;
        /* 00000064 */ Vector2 StoredPos;
        /* 0000006C */ DWORD field_6C;
        /* 00000070 */ DWORD field_70;
        /* 00000074 */ DWORD field_74;
        /* 00000078 */ SHORT field_78;
        /* 0000007A */ BYTE field_7A;
        /* 0000007B */ BYTE field_7B;
        /* 0000007C */ DWORD field_7C;
        /* 00000080 */ SHORT field_80;
        /* 00000082 */ BYTE field_82;
        /* 00000083 */ BYTE field_83;
        /* 00000084 */ INT  Timer;
        /* 00000088 */ DWORD field_88;
        /* 0000008C */ EntityAnimationData AnimData2;
        /* 000000A4 */ EntityAnimationData AnimData0;
        /* 000000BC */ EntityAnimationData AnimData3;
        /* 000000D4 */ EntityAnimationData AnimData11;
        /* 000000EC*/ EntityAnimationData AnimData4;
        /* 00000104 */ EntityAnimationData AnimData10;
        /* 0000011C */ EntityAnimationData AnimData5;
        /* 00000134 */ EntityAnimationData AnimData9;
        /* 0000014C */ EntityAnimationData AnimData6;
        /* 00000164 */ EntityAnimationData AnimData8;
        /* 0000017C */ EntityAnimationData AnimData7;

    };
    struct EntityDebugMode
    {
        /* 0x00000000 */ int blank;
        /* 0x00000004 */ short Sprites[0x100];
        /* 0x00000204 */ DWORD draw[0x100];
        /* 0x00000604 */ DWORD Spawn[0x100];
        /* 0x00000A04 */ EntityAnimationData DebugData;
        /* 0x00000A1C */ int ObjID;
        /* 0x00000A20 */ int DebugItemCount;
        /* 0x00000A24 */ DWORD PlayerActive;
        /* 0x00000A28 */ BYTE DebugItemSubType;
        /* 0x00000A29 */ BYTE SubTypeCount;
        /* 0x00000A2A */ BYTE field_A2a;
        /* 0x00000A2B */ BYTE field_A2B;
        /* 0x00000A2C */ BYTE field_A2C;
    };
    struct EntitySpikes : Entity
    {
        /* 0x00000058 */ void* State;
        /* 0x0000005C */ int Type;
        /* 0x00000060 */ int Moving;
        /* 0x00000064 */ BYTE Count;
        /* 0x00000065 */ BYTE Stagger;
        /* 0x00000066 */ short Timer;
        /* 0x00000068 */ short PlaneFilter;
        /* 0x0000006C */ DWORD dword6C;
        /* 0x00000070 */ DWORD gap70;
        /* 0x00000074 */ DWORD field_74;
        /* 0x00000078 */ DWORD field_78;
        /* 0x0000007C */ short field_7C;
        /* 0x0000007E */ BYTE field_7E;
        /* 0x0000007F */ BYTE field_7F;
        /* 0x00000080 */ short word80;
        /* 0x00000082 */ short word82;
        /* 0x00000084 */ short word84;
        /* 0x00000086 */ short word86;
        /* 0x00000088 */ EntityAnimationData Animation;

    };
    struct EntityUIPicture : Entity
    {
        /* 0x00000058 */ DWORD ListID;
        /* 0x0000005C */ DWORD FrameID;
        /* 0x00000060 */ DWORD Tag;
        /* 0x00000064 */ BYTE ZonePalette;
        /* 0x00000065 */ BYTE field_65;
        /* 0x00000066 */ BYTE field_66;
        /* 0x00000067 */ BYTE field_67;
        /* 0x00000068 */ DWORD ZoneID;
        /* 0x0000006C */ EntityAnimationData AnimData;
    };
    struct EntityUIButton : Entity {
        /* 00000058 */ DWORD field_58;
        /* 0000005C */ DWORD dword5C;
        /* 00000060 */ DWORD dword60;
        /* 00000064 */ DWORD field_64;
        /* 00000068 */ DWORD dword68;
        /* 0000006C */ DWORD dword6C;
        /* 00000070 */ DWORD dword70;
        /* 00000074 */ DWORD dword74;
        /* 00000078 */ DWORD dword78;
        /* 0000007C */ DWORD dword7C;
        /* 00000080 */ BYTE gap80[128];
        /* 00000100 */ DWORD Disabled;
        /* 00000104 */ SonicMania::Vector2 Size;
        /* 0000010C */ DWORD ListID;
        /* 00000110 */ DWORD FrameID;
        /* 00000114 */ DWORD Align;
        /* 00000118 */ DWORD ChoiceCount;
        /* 0000011C */ DWORD ChoiceDir;
        /* 0000011D */ DWORD Unused1;
        /* 0000011E */ DWORD Unused2;
        /* 0000011F */ DWORD Unused3;
        /* 00000120 */ DWORD Invisible;
        /* 00000124 */ DWORD AlignsP1;
        /* 00000128 */ DWORD FreeBindP2;
        /* 0000012C */ DWORD Transition;
        /* 00000130 */ DWORD StopMusic;
        /* 00000134 */ DWORD field_134;
        /* 00000138 */ DWORD dword138;
        /* 0000013C */ BYTE gap13C[16];
        /* 0000014C */ DWORD dword14C;
        /* 00000150 */ BYTE gap150[20];
        /* 00000164 */ SonicMania::EntityAnimationData entityanimationdata164l;
        /* 0000017C */ WORD word17C;
        /* 0000017E */ BYTE field_17E;
        /* 0000017F */ BYTE field_17F;
        /* 00000180 */ DWORD dword180;
        /* 00000184 */ DWORD dword184;
    };
    struct EntityUIVsRoundPicker : Entity
    {
        /* 00000058 */ DWORD gap58;
        /* 0000005C */ DWORD dword5C;
        /* 00000060 */ DWORD dword60;
        /* 00000064 */ BYTE Unknown1[156];
        /* 00000100 */ DWORD Disabled;
        /* 00000104 */ SonicMania::Vector2 Size;
        /* 0000010C */ DWORD Align;
        /* 00000110 */ DWORD ArrowWidth;
        /* 00000114 */ DWORD Val;
        /* 00000118 */ DWORD MaxVal;
        /* 0000011C */ BYTE gap11C[4];
        /* 00000120 */ DWORD dword120;
        /* 00000124 */ BYTE gap124[16];
        /* 00000134 */ DWORD dword134;
        /* 00000138 */ BYTE gap138[12];
        /* 00000144 */ SonicMania::EntityAnimationData AnimData1;
        /* 0000015C */ SonicMania::EntityAnimationData AnimData2;
        /* 00000174 */ SonicMania::EntityAnimationData AnimData3;
        /* 0000018C */ SonicMania::EntityAnimationData AnimData4;
        /* 000001A4 */ WORD word1A4;
        /* 000001A6 */ BYTE field_1A6;
        /* 000001A7 */ BYTE field_1A7;
    };
    struct EntityUIInfoLabel : Entity
    {
        /* 0x00000058 */ SonicMania::EntityAnimationData AnimData;
        /* 0x00000070 */ BYTE field_70;
        /* 0x00000071 */ BYTE field_71;
        /* 0x00000072 */ BYTE field_72;
        /* 0x00000073 */ BYTE field_73;
        /* 0x00000074 */ BYTE field_74;
        /* 0x00000075 */ BYTE field_75;
        /* 0x00000076 */ BYTE field_76;
        /* 0x00000077 */ BYTE field_77;
        /* 0x00000078 */ SonicMania::Vector2 Size;
        /* 0x00000080 */ wchar_t* Text;
        /* 0x00000084 */ int TextLength;
        /* 0x00000086 */ BYTE field_86;
        /* 0x00000087 */ BYTE field_87;
        /* 0x00000088 */ DWORD field_88;
        /* 0x0000008C */ SonicMania::EntityAnimationData AnimData2;
    };
    struct EntityUIVsResults : Entity
    {
        /* 00000058*/ DWORD State;
        /* 0000005C*/ BYTE  Unknown1[164];
        /* 00000100*/ DWORD Disabled;
        /* 00000104*/ DWORD PlayerID;
        /* 00000108*/ DWORD NumRows;
        /* 0000010C*/ DWORD TrophyCount;
        /* 00000110*/ BYTE  Row0Label;
        /* 00000111*/ BYTE  Row1Label;
        /* 00000112*/ BYTE  Row2Label;
        /* 00000113*/ BYTE  Row3Label;
        /* 00000114*/ BYTE  Row4Label;
        /* 00000115*/ BYTE  Row5Label;
        /* 00000116*/ BYTE  Row6Label;
        /* 00000117*/ BYTE  Row7Label;
        /* 00000118*/ BYTE  Row8Label;
        /* 00000119*/ BYTE  Row9Label;
        /* 0000011A*/ BYTE  Row10Label;
        /* 0000011B*/ BYTE  Row11Label;
        /* 0000011C*/ BYTE  Row12Label;
        /* 0000011D*/ BYTE  field_11D;
        /* 0000011E*/ BYTE  field_11E;
        /* 0000011F*/ BYTE  field_11F;
        /* 00000120*/ DWORD Row0Highlight;
        /* 00000124*/ DWORD Row1Highlight;
        /* 00000128*/ DWORD Row2Highlight;
        /* 0000012C*/ DWORD Row3Highlight;
        /* 00000130*/ DWORD Row4Highlight;
        /* 00000134*/ DWORD Row5Highlight;
        /* 00000138*/ DWORD Row6Highlight;
        /* 0000013C*/ DWORD Row7Highlight;
        /* 00000140*/ DWORD Row8Highlight;
        /* 00000144*/ DWORD Row9Highlight;
        /* 00000148*/ DWORD Row10Highlight;
        /* 0000014C*/ DWORD Row11Highlight;
        /* 00000150*/ DWORD Row12Highlight;
        /* 00000154*/ BYTE  Unknown2[104];
        /* 000001BC*/ DWORD dword1BC;
        /* 000001C0*/ BYTE  Unknown3[484];
        /* 000003A4*/ WORD  word3A4;
        /* 000003A6*/ BYTE  field_3A6;
        /* 000003A7*/ BYTE  field_3A7;
    };
    struct EntityUIControl : Entity
    {
        /* 0x00000058 */ DWORD field_58;
        /* 0x0000005C */ DWORD field_5C;
        /* 0x00000060 */ DWORD SelectedElement;
        /* 0x00000064 */ DWORD Tag;
        /* 0x00000068 */ DWORD field_68;
        /* 0x0000006C */ wchar_t* parentTag;
        /* 0x00000070 */ DWORD field_70;
        /* 0x00000074 */ DWORD ActiveOnLoad;
        /* 0x00000078 */ DWORD NoWidgets;
        /* 0x0000007C */ DWORD ResetSelection;
        /* 0x00000080 */ BYTE field_80;
        /* 0x00000081 */ BYTE byte81;
        /* 0x00000082 */ BYTE RowCount;
        /* 0x00000083 */ BYTE ColumnCount;
        /* 0x00000084 */ BYTE StartingID;
        /* 0x00000085 */ DWORD field_85;
        /* 0x00000086 */ DWORD field_86;
        /* 0x00000087 */ DWORD field_87;
        /* 0x00000088 */ Vector2 Size;
        /* 0x00000090 */ Vector2 CameraOffset;
        /* 0x00000098 */ Vector2 ScrollSpeed;
        /* 0x000000A0 */ DWORD NoClamp;
        /* 0x000000A4 */ DWORD NoWrap;
        /* 0x000000A8 */ DWORD dwordA;
        /* 0x000000AC */ DWORD dwordAC;
        /* 0x000000B0 */ DWORD gapB0[20];
        /* 0x000000C4 */ DWORD dwordC4;
        /* 0x000000C8 */ DWORD gapC8[4];
        /* 0x000000CC */ DWORD dwordCC;
        /* 0x000000D0 */ DWORD gapD0[4];
        /* 0x000000D4 */ DWORD dwordD4;
        /* 0x000000D8 */ DWORD field_D8[336];
    };
    struct EntityButton : Entity
    {
        /* 0x00000058 */ DWORD field_58;
        /* 0x0000005C */ DWORD field_5C;
        /* 0x00000060 */ DWORD field_60;
        /* 0x00000064 */ WORD field_64;
        /* 0x00000066 */ WORD field_66;
        /* 0x00000068 */ bool Pressed;
    };
#pragma endregion

#pragma region Object 

    struct Object
    {
        WORD ObjectID;
        BYTE EarlyUpdateFlag;
        BYTE field_3;
    };

    struct Obj_Ring : Object
    {
        Hitbox Hitbox;
        DWORD Pan;
        WORD SpriteIndex;
        WORD SFX_Ring;
    };

    struct Obj_Camera : Object
    {
        int XPos;
        int YPos;
    };
    struct Obj_Hud : Object
    {
        int Base;
        WORD HUDMappings;
        WORD SuperButtonMappings;
        WORD SFX_Click;
        WORD SFX_Starpost;
        DWORD dwordC;
        DWORD gap10;
        DWORD dword14; //Offset should be 14, but next jumps to 24!?
        DWORD field_24;
        DWORD field_28;
        DWORD field_2C;
        DWORD field_30;
        DWORD field_34;

    };
    struct Obj_Player : Object
    {
        int    SonicPhysicsTable[64];
        int    TailsPhysicsTable[64];
        int    KnuxPhysicsTable[64];
        int    MightyPhysicsTable[64];
        int    RayPhysicsTable[64];
        int    SuperPalette_Sonic[18];
        int    SuperPalette_Sonic_CPZ[18];
        int    SuperPalette_Sonic_HCZ[18];
        int    SuperPalette_Tails[18];
        int    SuperPalette_Tails_CPZ[18];
        int    SuperPalette_Tails_HCZ[18];
        int    SuperPalette_Knux[18];
        int    SuperPalette_Knux_CPZ[18];
        int    SuperPalette_Knux_HCZ[18];
        int    SuperPalette_Mighty[18];
        int    SuperPalette_Mighty_CPZ[18];
        int    SuperPalette_Mighty_HCZ[18];
        int    SuperPalette_Ray[18];
        int    SuperPalette_Ray_CPZ[18];
        int    SuperPalette_Ray_HCZ[18];
        int    field_93C;
        int    PlayerCount;
        short  UpState;
        short  DownState;
        short  LeftState;
        short  RightState;
        short  JumpPressState;
        short  JumpHoldState;
        int    field_950;
        int    field_954;
        int    FlyCarryPositions[32];
        BYTE   field_9D8;
        BYTE   field_9D9;
        BYTE   field_9DA;
        BYTE   field_9DB;
        BYTE   field_9DC;
        BYTE   field_9DD;
        BYTE   field_9DE;
        BYTE   field_9DF;
        BYTE   field_9E0;
        BYTE   field_9E1;
        BYTE   field_9E2;
        BYTE   field_9E3;
        int    field_9E4;
        BYTE   P2InputDelay;
        BYTE   field_9E9;
        BYTE   field_9EA;
        BYTE   field_9EB;
        BYTE   field_9EC;
        int    Rings;
        int    RingExtraLife;
        int    Powerups;
        int    SavedLives;
        int    SavedScore;
        int    SavedTargetScore;
        short  SonicSpriteIndex;
        short  SuperSpriteIndex;
        short  TailsSpriteIndex;
        short  TailsTailsSpriteIndex;
        short  KnuxSpriteIndex;
        short  MightySpriteIndex;
        short  RaySpriteIndex;
        short  SFX_Jump;
        short  SFX_Roll;
        short  SFX_Charge;
        short  SFX_Release;
        short  SFX_PeelCharge;
        short  SFX_PeelRelease;
        short  SFX_Dropdash;
        short  SFX_LoseRings;
        short  SFX_Hurt;
        short  SFX_PimPom;
        short  SFX_Skidding;
        short  SFX_Grab;
        short  SFX_Flying;
        BOOL   PlayingFlySFX;
        short  SFX_Tired;
        BYTE   field_A36;
        BYTE   field_A37;
        BOOL   PlayingTiredSFX;
        short  SFX_Land;
        short  SFX_Slide;
        short  SFX_Outtahere;
        short  SFX_Transform2;
        short  SFX_Swap;
        short  SFX_SwapFail;
        short  SFX_MightyDeflect;
        short  SFX_MightyDrill;
        short  SFX_MightyLand;
        short  SFX_MightyUnspin;
        int    RaySwoopTimer;
        int    RayDiveTimer;
        int    GotHit;
        int    field_A5C;
        int    field_A60;
        int    field_A64;
        void* pfuncA68;
        void* field_A6C;
        int    field_A70;
    };
    struct Obj_UFO_Player : Object
    {
        int Base;
        DWORD field_4;
        BYTE field_8;
        BYTE field_9;
        BYTE field_A;
        BYTE field_B;
        WORD SFX_Jump;
        WORD SFX_Charge;
        WORD SFX_Release;
        WORD SFX_LoseRings;
        WORD SFX_Drop;
        WORD SFX_Bumper;
        WORD SFX_Spring;
        WORD SFX_Skid;
        WORD SFX_GrittyGround;
        WORD JogModel;
        WORD DashModel;
        WORD JumpModel;
        WORD BallMOdel;
        WORD TumbleModel;
        WORD ModelID;
        BYTE field_2A;
        BYTE field_2B;

    };
    struct Obj_Platform : Object
    {
        short SpriteIndex;
        short field_6;
        int   StoodPositions[8];
        short SFX_Clacker;
        short SFX_Clang;
        short SFX_Push;
        short SFX_Clack;
        int   dword30;
        int   dword34;
    };
    struct Obj_Dust : Object
    {
        WORD SpriteIndex;
    };
    struct Obj_Clapperboard : Object
    {
        WORD SpriteIndex;
        WORD SFX_Whack;
    };
    struct Obj_DebugMode : Object
    {
        WORD Sprites[0x100];
        int (*DrawPtrs[0x100])();
        Entity* (*SpawnPtrs[0x100])();
        EntityAnimationData DebugData;
        DWORD ObjID;
        DWORD DebugItemCount;
        DWORD PlayerActive;
        BYTE DebugItemSubType;
        BYTE SubtypeCount;
        BYTE field_A2A;
        BYTE field_A2B;
        BYTE field_A2C;
    };
    struct Obj_PauseMenu : Object
    {
        short SFX_Bleep;
        short SFX_Accept;
        DWORD dword8;
        DWORD dwordC;
        DWORD dword10;
        DWORD dword14;
        DWORD dword18;
        BYTE field_1C;
        BYTE field_1D;
        BYTE field_1E;
        BYTE field_1F;
        BYTE field_20;
        BYTE field_21;
        BYTE field_22;
        BYTE field_23;
        BYTE field_24;
        BYTE field_25;
        BYTE field_26;
        BYTE field_27;
        BYTE field_28;
        BYTE field_29;
        BYTE field_2A;
        BYTE field_2B;
        BYTE field_2C;
        BYTE field_30;
        BYTE field_31;
        BYTE field_32;
        BYTE field_33;
        BYTE field_34;
        BYTE field_35;
        BYTE field_36;
        BYTE field_37;
        BYTE field_38;
        BYTE field_39;
        BYTE field_3A;
        BYTE field_3B;
        BYTE field_3C;
        BYTE field_40;
        BYTE field_41;
        BYTE field_42;
        BYTE field_43;
        BYTE field_44;
        BYTE field_45;
        BYTE field_46;
        BYTE field_47;
        BYTE field_48;
        BYTE field_49;
        BYTE field_4A;
        BYTE field_4B;
        BYTE field_4C;
        BYTE field_50;
        BYTE field_51;
        BYTE field_52;
        BYTE field_53;
        BYTE field_54;
        BYTE field_55;
        BYTE field_56;
        BYTE field_57;
        BYTE field_58;
        BYTE field_59;
        BYTE field_5A;
        BYTE field_5B;
        DWORD InkValues[0xFFFF];
    };
    struct Obj_FXRuby : Object
    {
        DWORD UnknownArray[0x200];
        short InkValues[0xFFFF];
        DWORD FGLow;
        DWORD FGHigh;
    };
    struct Obj_LoveTester : Object
    {
        Hitbox Hitbox1;
        Hitbox Hitbox2;
        Hitbox Hitbox3;
        short SpriteIndex;
        short SFXScore;
        DWORD Field_20;
        DWORD Field_24;
        DWORD Field_28;
        DWORD Field_2C;
        Vector2 Positions;

    };
    struct Obj_ForceSpin : Object
    {
        //TODO: fill out later
    };

    struct Obj_CollapsingPlatform : Object
    {
        //TODO: fill out later
    };

    struct Obj_LRZBreakableWall : Object
    {
        //TODO: fill out later
    };

    struct Obj_BreakableWall : Object
    {
        //TODO: fill out later
    };

    struct Obj_StarPost : Object
    {
        //TODO: fill out later
    };

    struct Obj_Button : Object
    {
        //TODO: fill out later
    };

    struct Obj_Door : Object
    {
        //TODO: fill out later
    };



    struct Obj_Spikes : Object
    {
        int field_4;
        EntityAnimationData VerticalAnimData;
        EntityAnimationData HorizontalAnimData;
        short SpriteIndex;
        BYTE field_3A;
        BYTE field_3B;
        int dword3C;
        int dword40;
        short SFX_Move;
        short SFX_Spke;
    };

    struct Obj_Explosion : Object
    {
        short SpriteIndex;
        short SFX_Destroy;
    };

    struct Obj_ScoreBonus : Object
    {
        short SpriteIndex;
    };

    struct Obj_Animals : Object
    {
        //TO-DO: fill out later
    };

    struct Obj_Spring : Object
    {
        short SpriteIndex;
        short SFX_Spring;
    };

    struct Obj_ItemBox : Object
    {
        Hitbox MainHitbox;
        Hitbox HiddenHitbox;
        int BrokenFrame;
        short SpriteMappings;
        short SFX_Destroy;
        short SFX_Teleport;
        short SFX_HyperRing;
        short SFX_PowerDown;
        short SFX_Revovery;
    };

    struct Obj_SpecialRing : Object
    {
        short SpriteIndex;
        Hitbox Hitbox;
        short  field_E;
        short  SFX_Warp;
        short  ModelIndex;
        short  View_SpecialRing;
    };

    struct Obj_PlaneSwitch : Object
    {
        //TO-DO: fill out later
    };
    struct Obj_GHZSetup : Object
    {
        int WaterFadeAmount;
        int DelayA;
        int FrameA;
        int DelayListA[0x08];
        int DelayB;
        int FrameB;
        int DelayListB[16];
        ushort AniTiles;
    };
    struct Obj_LRZ1Setup : Object
    {
        int PalTimer;
        int FadeTimer;
        DWORD* BG1;
        DWORD* BG2;
        DWORD* FGLow;
        DWORD* FGHigh;
        int FGArray[0x20];
        int BGArray[0x20];
    };

    struct Obj_UIPicture : SonicMania::Object
    {
        /* 0x00000004 */ WORD SpriteIndex;
    };

    struct Obj_UIText : SonicMania::Object
    {
        /* 0x00000004 */ WORD SpriteIndex;
    };

    struct Obj_ZONE : Object
    {
        DWORD Act2;
        void* PostStageFinishEvent;
        DWORD field_C;
        DWORD field_10;
        DWORD field_14;
        DWORD field_18;
        DWORD field_1C;
        DWORD field_20;
        DWORD field_24;
        DWORD field_28;
        DWORD field_2C;
        DWORD field_30;
        DWORD field_34;
        DWORD field_38;
        DWORD field_3C;
        DWORD field_40;
        DWORD field_44;
        DWORD field_48;
        DWORD field_4C;
        DWORD field_50;
        DWORD field_54;
        DWORD field_58;
        DWORD field_5C;
        DWORD field_60;
        DWORD field_64;
        DWORD field_68;
        DWORD field_6C;
        DWORD field_70;
        DWORD field_74;
        DWORD RingFrame;
        DWORD Timer;
        DWORD Timer2;
        DWORD field_84[4];
        DWORD field_94[4];
        DWORD field_A4[4];
        DWORD field_B4[4];
        DWORD field_C4[4];
        DWORD field_D4[4];
        DWORD field_E4[4];
        DWORD field_F4[4];
        DWORD field_104[4];
        DWORD field_114;
        BYTE field_118;
        BYTE field_119;
        BYTE field_11A;
        BYTE field_11B;
        BYTE field_11C;
        BYTE field_11D;
        BYTE field_11E;
        BYTE field_11F;
        BYTE field_120;
        BYTE field_121;
        BYTE field_122;
        BYTE field_123;
        DWORD field_124;
        DWORD field_128;
        BYTE field_12C;
        BYTE field_12D;
        BYTE field_12E;
        BYTE field_12F;
        BYTE field_130;
        BYTE field_131;
        BYTE field_132;
        BYTE field_133;
        BYTE field_134;
        BYTE field_135;
        BYTE field_136;
        BYTE field_137;
        BYTE field_138;
        BYTE field_139;
        BYTE field_13A;
        BYTE field_13B;
        BYTE field_13C;
        BYTE field_13D;
        BYTE field_13E;
        BYTE field_13F;
        BYTE field_140;
        BYTE field_141;
        BYTE field_142;
        BYTE field_143;
        DWORD field_144;
        BYTE field_148;
        BYTE field_149;
        BYTE field_14A;
        BYTE field_14B;
        BYTE field_14C;
        BYTE field_14D;
        BYTE field_14E;
        BYTE field_14F;
        BYTE field_150;
        BYTE field_151;
        BYTE field_152;
        BYTE field_153;
        DWORD field_154;
        DWORD field_158;
        DWORD field_15C;
        DWORD field_160;
        WORD FGLayers;
        WORD FGLow;
        WORD FGHigh;
        WORD MoveLayer;
        WORD ScratchLayer;
        WORD FGLow_ID;
        WORD FGHigh_ID;
        WORD Move_ID;
        BYTE field_174;
        BYTE field_175;
        BYTE DrawOrderLow;
        BYTE DrawOrderHigh;
        BYTE PlayerHighLayer;
        BYTE PlayerLowLayer;
        BYTE field_17A;
        BYTE field_17B;
        WORD SFX_Fail;
        BYTE data[17826];
        DWORD field_4720;
        DWORD field_4724;
        DWORD TimeStart;
    };

    struct CompetitionSession
    {
        int Unknown_Gap_0[22];
        int inMatch;
        int NumberOfPlayers;
        int Unknown_60;
        int Unknown_64;
        int Unknown_68;
        int CurrentRound;
        int TotalRounds;
        int MonitorMode;
        int Unknown_78;
        int Unknown_Gap_7C[12];
        BYTE CharacterFlags[4];
        BYTE WinnerFlag[12];
        int Rings_P1;
        int Rings_P2;
        int Rings_P3;
        int Rings_P4;
        int Score_P1;
        int Score_P2;
        int Score_P3;
        int Score_P4;
        int Items_P1;
        int Items_P2;
        int Items_P3;
        int Items_P4;
        int TimeMinutes_P1;
        int TimeSeconds_P1;
        int TimeCentiseconds_P1;
        int TimeMinutes_P2;
        int TimeSeconds_P2;
        int TimeCentiseconds_P2;
        int TimeMinutes_P3;
        int TimeSeconds_P3;
        int TimeCentiseconds_P3;
        int TimeMinutes_P4;
        int TimeSeconds_P4;
        int TimeCentiseconds_P4;
        int FinishFlags;
        int TotalRings_P1;
        int TotalRings_P2;
        int TotalRings_P3;
        int TotalRings_P4;
        int Wins_P1;
        int Wins_P2;
        int Wins_P3;
        int Wins_P4;
        int InitalLives_P1;
        int InitalLives_P2;
        int InitalLives_P3;
        int InitalLives_P4;
        int Unknown_150;
        int Unknown_154;
        int Unknown_156;
        int Unknown_15C;
        int ReadOnlyDisplayMode;
        int Unknown_Gap_164[4007];
    };

    struct GameOptions {
        int GameMode;
        int PlayerID;
        int SpecialCleared;
        int SpecialRingID;
        int BlueSpheresID;
        int BlueSpheresInit;
        int  AtlEnabled; // Act Transition Load
        int  AtlEntityCount; // An Entity Bank Basically
        int  AtlEntitySlot[0x20];// Entity SlotIDs
        int AtlEntityData[0x4000];// Entities
        int  SaveLoaded;
        int  SaveRAM[0x4000];
        int  SaveSlotID;
        int  NoSaveSlot[0x400];
        int  MenuParam[0x4000];
        int  ItemMode;
        int SuppressTitlecard;
        int SuppressAutoMusic;
        CompetitionSession  CompetitionSession;
        int  MedalMods; // Stuff Like Instashield and etc (Bonus Stuff that medals unlock)
        int  ParallaxOffset[256];
        int  EnableIntro;
        int  OptionsLoaded;
        int  OptionsRAM[0x80];
        int  PresenceID;
        int  MedallionDebug;
        int  NoSave;
        int  NotifiedAutosave;
        int RecallEntities;
        int  RestartRings;
        int  Restart1UP;
        int  RestartShield;
        int  RestartPos[4];
        int  RestartSlot[4];
        int  RestartDir[4];
        int  RestartMinutes;
        int  RestartSeconds;
        int  RestartMilliseconds;
        int  TempMinutes;
        int  TempSeconds;
        int  TempMilliseconds;
        int  RestartScore;
        int  RestartScore1UP;
        int  RestartLives[4];
        int  RestartMusicID;
        int RestartFlags;
        int  TempFlags;
        int  Continues;
        int  InitCoolBonus;
        int  CoolBonus[4];
        int  ReplayWriteBuffer[0x40000];
        int  ReplayReadBuffer[0x40000];
        int  ReplayTempWBuffer[0x40000];
        int  ReplayTempRBuffer[0x40000];
        int  ReplayTableID;
        int  ReplayTableLoaded;
        int  TATableID;
        int  TATableLoaded;
        int  Stock; // Encore Lives
        int  CharacterFlags;
        int  VapeMode; // Vape Mode
        int  Secrets;
        int  SuperSecret;
        int  SuperMusicEnabled; // Play Super Music when going super?
        int  LastHasPlus;
        int  HasPlusInitial;
    };

#pragma endregion

#pragma region Pointer Functions
    static byte GetChaosEmeraldBits()
    {
        int* pointer = (int*)(baseAddress + 0x00AC6A20);
        if (!*pointer)
            return 0;
        pointer = (int*)(*pointer + 0x10);
        if (!*pointer)
            return 0;
        pointer = (int*)(*pointer + 0x70);
        if (!*pointer)
            return 0;
        return *pointer;
    }

    static byte GetChaosEmeraldCount()
    {
        int CECount = 0;
        byte CE = GetChaosEmeraldBits();
        if (CE == 0)
            return 0;
        for (int i = 0; i < 7; ++i)
            if (CE & (1 << i))
                ++CECount;
        return CECount;
    }
#pragma endregion

#pragma region HelperFunctions

    // Used to set the strings of UIText and UIInfoLabels
    inline void ConvertASCII2Unicode(wchar_t* dest, char* src, size_t size, int offset)
    {
        memset(dest, 0, size * 2);
        for (int i = 0; i < size; ++i)
            dest[i] = src[i] + offset;
    }

    inline void SetVSScreenCount(int count)
    {
        BYTE* Pointer = *(BYTE**)((baseAddress + 0xA530FC));
        WriteData((BYTE*)baseAddress + 0xA530FC, (BYTE)count);
    }

    inline BYTE* GetUIBGPointer()
    {
        return *(BYTE**)(baseAddress + 0xAC68E4);
    }

    inline void WriteUIBG_Color(int offset, int r, int g, int b)
    {
        unsigned int color = r << 16
            | g << 8 | b; // idk if this is reversed
        WriteData((unsigned int*)(GetUIBGPointer() + offset), color);
    }

    inline void SetUIBG_BGColor(int r, int g, int b)
    {
        WriteUIBG_Color(4, r, g, b);
    }

    inline void SetUIBG_FGLowColor(int r, int g, int b)
    {
        WriteUIBG_Color(8, r, g, b);
    }

    inline void SetUIBG_FGHighColor(int r, int g, int b)
    {
        WriteUIBG_Color(12, r, g, b);
    }

    // Written by Sajid
    inline SHORT ToRGB565(byte R, byte G, byte B)
    {
        //return ((R & 0b11111000) << 8) | ((G & 0b11111100) << 3) | (B >> 3);
        return (B >> 3) | ((G >> 2) << 5) | ((R >> 3) << 11);
    }

    inline SHORT ManiaRGB565(byte R, byte G, byte B)
    {
        int v9 = 0;
        SHORT result;
        do
        {
            R = (v9 & 0xFFF8) << 8;
            result = v9 >> 3;
            G = 8 * (v9 & 0xFFFC);
            B = v9 >> 3;
            ++v9;
        } while (v9 < 256);
        return result;
    }

    inline void ToRGB888(SHORT val, byte& R, byte& G, byte& B)
    {
        R = (val & 0b1111100000000000) >> 8;
        G = (val & 0b0000011111100000) >> 3;
        B = (val & 0b0000000000011111) << 3;
    }

    inline SHORT ToRGB565(int val)
    {
        return ToRGB565((val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF);
    }

    inline int ToRGB888(SHORT val)
    {
        byte oR = (val & 0xF800) >> 11;
        byte oG = (val & 0x7E0) >> 5;
        byte oB = (val & 0x1F);

        byte R = (oR * 527 + 23) >> 6;
        byte G = (oG * 259 + 33) >> 6;
        byte B = (oB * 527 + 23) >> 6;
        return ((R & 0xFF) << 16) | ((G & 0xFF) << 8) | (B & 0xFF);
    }

    inline int Blend(int blended, int overlay, float a)
    {
        byte OR = (blended >> 16) & 0xFF;
        byte OG = (blended >> 8) & 0xFF;
        byte OB = blended & 0xFF;

        byte NR = (overlay >> 16) & 0xFF;
        byte NG = (overlay >> 8) & 0xFF;
        byte NB = overlay & 0xFF;

        byte R = (byte)(a * NR + (1 - a) * OR);
        byte G = (byte)(a * NG + (1 - a) * OG);
        byte B = (byte)(a * NB + (1 - a) * OB);

        return ((R & 0xFF) << 16) | ((G & 0xFF) << 8) | (B & 0xFF);
    }

    inline void WriteRGB888ToRGB565Array(SHORT* writeAddress, byte* colors, int colorCount)
    {
        for (int i = 0; i < colorCount; ++i)
            writeAddress[i] = ToRGB565(colors[(i * 3) + 0], colors[(i * 3) + 1], colors[(i * 3) + 2]);
    }

    inline SHORT* GetColour(SHORT* Palette, int x, int y)
    {
        return Palette + x + 16 * y;
    }

    inline int* GetAddress(int baseAddress, int offset1)
    {
        return (int*)((*(int*)baseAddress) + offset1);
    }

    inline int* GetAddress(int baseAddress, int offset1, int offset2)
    {
        return (int*)(*(int*)GetAddress(baseAddress, offset1) + offset2);
    }

    static ushort GetSpritePointer(int SpritePointer, int offset)
    {
        int* pointer = (int*)(baseAddress + SpritePointer);
        if (!*pointer)
            return 0;
        pointer = (int*)(*pointer + offset);
        if (!*pointer)
            return 0;
        return *pointer;
    }
    static INT GetPointer(int SpritePointer, int offset) //CnG Uses this everywhere, tired of copying it into every header. 
    {
        int* pointer = (int*)(baseAddress + SpritePointer);
        if (!*pointer)
            return 0;
        pointer = (int*)(*pointer + offset);
        if (!*pointer)
            return 0;
        return *pointer;
    }
#pragma endregion

#pragma region ManiaHelperFunctions

    inline Character GetCharacter(int playerId)
    {
        return *((Character*)GetAddress((baseAddress + 0x00AA763C), 4 + playerId));
    }

    inline Character* GetCharacter_ptr(int playerId)
    {
        return ((Character*)GetAddress((baseAddress + 0x00AA763C), 4 + playerId));
    }

    template <typename T>
    inline T* GetActiveEntity()
    {
        return (T*)EntityInfo.CurrentEntity;
    }

    template <typename T>
    inline T* GetEntityFromSceneSlot(int slot)
    {
        return (T*)((baseAddress + 0x0047B010) + (slot * 0x458));
    }

    template <typename T>
    static T* GetAbsAddress(int jmp)
    {
        int r = *(int*)(jmp + 1);
        int abs = jmp + r + 5;
        return (T*)abs;
    }

    inline short GetObjectIDFromType(ObjectType type)
    {
        if (*((int*)(baseAddress + type)) == 0)
            return 0;
        return *(short*)GetAddress((int)(baseAddress + type), 0);
    }

    inline Ability GetMoveSetByCharacter(Character character)
    {
        switch (character)
        {
        case Character_Sonic:
            return MOVESET_SONIC;
        case Character_Tails:
            return MOVESET_TAILS;
        case Character_Knux:
            return MOVESET_KNUX;
        case Character_Mighty:
            return MOVESET_MIGHTY;
        case Character_Ray:
            return MOVESET_RAY;
        default:
            return MOVESET_SONIC;
        }
    }

    inline void RestartScene()
    {
        // Setting GameState to NotRunning restarts the scene
        GameState = GameState_NotRunning;
    }

    inline void ChangeScene(Scene scene)
    {
        CurrentScene = scene;
        GameState = GameState_NotRunning;
    }

    inline Entity* SpawnObject(short objectID, short subObject, short x, short y)
    {
        return SpawnObject_Internal(objectID, subObject, ((int)x) << 16, ((int)y) << 16);
    }

    inline Entity* SpawnObject(short objectID, short subObject, Vector2 position)
    {
        return SpawnObject(objectID, subObject, position.X, position.Y);
    }

    inline void DespawnEntity(Entity* entity)
    {
        DespawnEntity_Internal(entity, 0, 0);
    }

    inline int ShakeCamera(int ShakeX, int CameraTarget, int ShakeY)
    {
        int result; // eax
        EntityCamera* Camera; // [esp+8h] [ebp-4h]

        if (OBJ_Camera)
        {
            Camera = NULL;
            result = GetActiveObjects(OBJ_Camera->ObjectID, (Entity*)&Camera);
            if (result == 1)
            {
                while (Camera->ActiveEntity != CameraTarget)
                {
                    result = GetActiveObjects(OBJ_Camera->ObjectID, (Entity*)&Camera);
                    if (result != 1)
                        return result;
                }
                Camera->ShakeX = ShakeX;
                Camera->ShakeY = ShakeY;
                //result = RSDK_Unknown45();
            }
        }
        return result;
    }

    inline void PlaySong(const char* filePath, int loopstart, bool loop)
    {
        int* addr = GetAddress(0x00AC6E08, 0x248);
        if ((int)addr > 0x248)
            *addr = PlayMusic(filePath, 0, 0, loopstart, loop ? 1 : 0);
        else
            PlayMusic(filePath, 0, 0, loopstart, loop ? 1 : 0);
    }

    inline int PlaySoundFXS(const char* path)
    {
        return PlaySoundFX(GetSoundFXID(path), 0, 0xFF);
    }

    static int loc_LoadSoundFX = baseAddress + 0x001BBA00;
    static _declspec(naked) short LoadSoundFX(const char* path, Scope scope)
    {
        __asm
        {
            mov dl, 1
            mov ecx, [ESP + 4]
            push[ESP + 8]
            call loc_LoadSoundFX
            add esp, 4
            ret
        }
    }

    static short TryLoadSoundFX(const char* path, Scope scope)
    {
        if (GetSoundFXID(path) == (short)-1)
            return LoadSoundFX(path, scope);
        return -1;
    }


    static int loc_HitEntity = (baseAddress + 0x000C9380);
    inline __declspec(naked) bool HitEntity(Entity* attacker, Entity* target, DWORD a3)
    {
        __asm
        {
            mov edx, [ESP + 4]
            mov ecx, [ESP + 8]
            push[ESP + 12]
            call loc_HitEntity
            add esp, 4
            ret
        }
    }
    static int loc_CallDespawn = (baseAddress + 0x001D3CF0);
    inline __declspec(naked) bool CallDespawn(EntityPlayer* Player, Entity* target)
    {
        __asm
        {
            mov edx, [ESP + 4]
            mov ecx, [ESP + 8]
            push[ESP + 12]

            mov edi, [ESP + 0x04]
            push edi

            push 0x12
            push eax

            call loc_CallDespawn
            add esp, 4
            ret
        }
    }

#pragma endregion

#pragma region ManiaPatches

    inline void BindLBAndRB()
    {
        // LB
        byte LBPatch[6]{ (byte)0x09, (byte)0x42, (byte)0x4C, (byte)0x90, (byte)0x90, (byte)0x90 };
        WriteData((byte*)(baseAddress + 0x001E6358), LBPatch, 6);
        // RB
        byte RBPatch[6]{ (byte)0x09, (byte)0x42, (byte)0x70, (byte)0x90, (byte)0x90, (byte)0x90 };
        WriteData((byte*)(baseAddress + 0x001E6362), RBPatch, 6);
    }

#pragma endregion

#pragma region DevMenu

    enum DevMenu_Alignment : int
    {
        Alignment_Left,
        Alignment_Center,
        Alignment_Right
    };

    DataPointer(bool, DevMenu_Enabled, 0x002FC867);
    DataPointer(void*, DevMenu_Address, 0x002FBB40);
    DataPointer(int, DevMenu_Option, 0x002FBB44);
    DataPointer(int, DevMenu_Scroll, 0x002FBB48);
    FunctionPointer(int, DevMenu_DrawRect, (int x, int y, signed int width, int height, int color, signed int a6, int a7, int a8), 0x001D8870);
    FunctionPointer(int, DevMenu_DrawText, (int x, const char* text, int y, DevMenu_Alignment CenterText, int color), 0x1D58C0);
    DataPointer(DWORD, dword_D3CC00, 0x947C6C);

    FunctionPointer(char, Devmenu_MainMenu, (), 0x001C2500);
    FunctionPointer(char, Devmenu_StageSelect, (), 0x001C2B60);
    FunctionPointer(char, Devmenu_SceneSelect, (), 0x001C2DB0);
    FunctionPointer(char, Devmenu_Options, (), 0x001C3090);


    inline int DrawDevText(int x, int y, DevMenu_Alignment CenterText, int color, const char* format, ...)
    {
        return DevMenu_DrawText(x, format, y, CenterText, color);
    }

    static int loc_DrawText = baseAddress + 0x1D58C0;
    static __declspec(naked) WORD DevMenu_DrawTextASM(int XPosition, const char* text, int YPosition, DevMenu_Alignment alignment, int color)
    {
        __asm
        {
            mov edx, [ESP + 4]
            mov ecx, [ESP + 8]
            push[ESP + 20]
            push[ESP + 20]
            push[ESP + 20]
            call loc_DrawText
            add esp, 12
            ret
        }
    }

#pragma endregion
}