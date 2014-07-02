#include "sys_main_header.h"
#include "sys_window.h"
#include "..\render\r_common.h"
#include "..\framework\log.h"
#include "..\render\r_file.h"
#include "..\Game\Camera.h"
#include "..\Game\level.h"
#include "..\Game\Player.h"
#include "..\Game\weapon.h"

static MainGameWindow gameWindow;

extern Render render;
extern Logger logger;

int vKeys[ 4 ] = { 0 };

#define SX_TO_GX( v, x, k )	( v ).x = ( ( ( v ).z + ( k ) ) * ( x ) / ( k ) )
#define SY_TO_GY( v, y, k )	( v ).y = ( ( ( v ).z + ( k ) ) * ( y ) / ( k ) )

typedef struct {
	int p_x, p_y;
	int x, y;
} m_pos_t;

m_pos_t m_pos;

bool mmove = false;

LRESULT CALLBACK	WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg ) {
	case WM_KEYDOWN:
		switch ( wParam ) {
		case VK_UP:
			vKeys[ 0 ] = 1; break;
		case VK_DOWN:
			vKeys[ 1 ] = 1; break;
		case VK_LEFT:
			vKeys[ 2 ] = 1; break;
		case VK_RIGHT:
			vKeys[ 3 ] = 1; break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch ( wParam ) {
		case VK_UP:
			vKeys[ 0 ] = 0;
			break;
		case VK_DOWN:
			vKeys[ 1 ] = 0;
			break;
		case VK_LEFT:
			vKeys[ 2 ] = 0;
			break;
		case VK_RIGHT:
			vKeys[ 3 ] = 0;
			break;
		default:
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if( !mmove ) {
			m_pos.x = GET_X_LPARAM( lParam );
			m_pos.y = GET_Y_LPARAM( lParam );
			SetCursorPos( m_pos.p_x, m_pos.p_y );
			SetCursor( FALSE );
			mmove = true;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

static int RegGameClass( void ) 
{
	WNDCLASSEX wc;

	ZeroMemory( &wc, sizeof( wc ) );
	wc.cbSize = sizeof( wc );
	wc.lpszClassName = CLASS_NAME;
	wc.lpfnWndProc = ( WNDPROC )WindowProc;

	return RegisterClassEx( &wc );

}


static void CreateGameWindow( void )
{
	gameWindow.width = GetSystemMetrics( SM_CXSCREEN );
	gameWindow.height = GetSystemMetrics( SM_CYSCREEN );

	gameWindow.hWnd = CreateWindow( CLASS_NAME, 
									"Qoom",
									WS_POPUP, 
									0, 0, 
									gameWindow.width, 
									gameWindow.height, 
									( HWND )NULL, 
									( HMENU )NULL, gameWindow.hInstance, 
									0 
								   );

	return ;


}



INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

	gameWindow.hInstance = hInstance;

	if ( RegGameClass() == 0 ) {
		return -1;
	}

	
	CreateGameWindow();
	if ( gameWindow.hWnd == 0 ) {
		return -2;
	}

	ShowWindow( gameWindow.hWnd, SW_SHOW );
	SetFocus( gameWindow.hWnd );
	m_pos.x = m_pos.p_x = gameWindow.width / 2;
	m_pos.y = m_pos.p_y = gameWindow.height / 2;

	SetCursorPos( m_pos.p_x, m_pos.p_y );
	SetCursor( FALSE );
	UpdateWindow( gameWindow.hWnd );

	gameWindow.done = FALSE;

	MSG msg;
	render.Init( gameWindow, 1.0f, 2048.0f, D3DX_PI / 2  );
	Player player;
	GameObject railgun;
	railgun.LoadFromFile( "models\\weapons2\\railgun\\railgun.md3" );
	railgun.SetScale( 1.0f );
	railgun.SetPos( Vector3( 325.0f, 25.0f, 325.0f ) );
	railgun.SetVertexShader( "shaders\\weapon.vsh" );
	railgun.SetPixelShader( "shaders\\weapon.psh" );
	railgun.SetPlayer( &player );
	railgun.ActivateCM();
	railgun.ActivateR();

	GameObject bfg;
	bfg.LoadFromFile( "models\\weapons2\\bfg\\bfg.md3" );
	bfg.SetScale( 1.0f );
	bfg.SetPos( Vector3( 125.0f, 25.0f, 125.0f ) );
	bfg.SetVertexShader( "shaders\\weapon.vsh" );
	bfg.SetPixelShader( "shaders\\weapon.psh" );
	bfg.SetPlayer( &player );
	bfg.ActivateCM();
	bfg.ActivateR();

	GameObject plasma;
	plasma.LoadFromFile( "models\\weapons2\\plasma\\plasma.md3" );
	plasma.SetScale( 1.0f );
	plasma.SetPos( Vector3( 125.0f, 25.0f, 225.0f ) );
	plasma.SetVertexShader( "shaders\\weapon.vsh" );
	plasma.SetPixelShader( "shaders\\weapon.psh" );
	plasma.SetPlayer( &player );
	plasma.ActivateCM();
	plasma.ActivateR();

	GameObject rocketl;
	rocketl.LoadFromFile( "models\\weapons2\\rocketl\\rocketl.md3" );
	rocketl.SetScale( 1.0f );
	rocketl.SetPos( Vector3( 225.0f, 25.0f, 125.0f ) );
	rocketl.SetVertexShader( "shaders\\weapon.vsh" );
	rocketl.SetPixelShader( "shaders\\weapon.psh" );
	rocketl.SetPlayer( &player );
	rocketl.ActivateCM();
	rocketl.ActivateR();

	GameObject brain;
	brain.LoadFromFile( "models\\gibs\\brain.md3" );
	brain.SetScale( 1.0f );
	brain.SetPos( Vector3( 275.0f, 5.0f, 125.0f ) );
	brain.SetVertexShader( NULL );
	brain.SetPixelShader( NULL );
	brain.SetPlayer( &player );
	brain.ActivateCM();
	brain.ActivateR();

	GameObject leg;
	leg.LoadFromFile( "models\\gibs\\leg.md3" );
	leg.SetScale( 0.5f );
	leg.SetPos( Vector3( 275.0f, 5.0f, 155.0f ) );
	leg.SetVertexShader( NULL );
	leg.SetPixelShader( NULL );
	leg.SetPlayer( &player );
	leg.ActivateCM();
	leg.ActivateR();

	GameObject shotgun;
	shotgun.LoadFromFile( "models\\weapons2\\shotgun\\shotgun.md3" );
	shotgun.SetScale( 1.0f );
	shotgun.SetPos( Vector3( 275.0f, 5.0f, 225.0f ) );
	shotgun.SetVertexShader( NULL );
	shotgun.SetPixelShader( NULL );
	shotgun.SetPlayer( &player );
	shotgun.ActivateCM();
	shotgun.ActivateR();

	Weapon wep;
	wep.LoadFromFile( "models\\weapons2\\rocketl\\rocketl.md3" );

	wep.SetPos( 3.0f, -5.0f, -5.5f );
	wep.SetScale( 0.5f );
	wep.SetRot( -( 8 * D3DX_PI ) / 14, -D3DX_PI / 2, D3DX_PI / 8 );

	Level level;
	Vector3 pos = level.GenerateRandomLevel( 12, 12, 50.0f, 50.0f, (int)time( NULL ) );
	//level.CreateTestLevel();
	level.AddObject( railgun );
	level.AddObject( bfg );
	level.AddObject( plasma );
	level.AddObject( rocketl );
	level.AddObject( brain );
	level.AddObject( leg );
	level.AddObject( shotgun );
	player.SetWeapon( &wep );
	player.SetSize( 10.0f, 15.0f, 8.0f );
	player.SetPos( pos.x, 30.0f, pos.z );
	//player.SetPos( 0.0f, 8.0f, 25.0f );
	player.SetCMSize( 7.5f );
	player.SetLevel( &level );

	while( !gameWindow.done ) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if( msg.message == WM_QUIT ) {
				gameWindow.done = TRUE;
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		} else {
			render.Set3D();
			UINT iTime = timeGetTime();
			FLOAT fAngle = iTime * ( 2.0f * D3DX_PI ) / 7000.0f;
			D3DXMATRIX world, cur, view, world_t;

			float dx = 0.0f, dy = 0.0f, dz = 0.0f;
			if ( vKeys[ 0 ] || vKeys[ 1 ] || vKeys[ 2 ] || vKeys[ 3 ] ) {		// <-- || -->
				if ( vKeys[ 0 ] || vKeys[ 1 ] ) {
					dz = 1.0f;
					dz = vKeys[ 1 ] ? -dz : dz;
				}
				if ( vKeys[ 2 ] || vKeys[ 3 ] ) {
					dx = 1.0f;
					dx = vKeys[ 2 ] ? -dx : dx ;
				}
				player.Move( dx, dy, dz );
			}
			if ( mmove ) {
				mmove = false;
				player.Rotation( ( m_pos.x - m_pos.p_x ) * 2 * D3DX_PI / ( gameWindow.width * 10 ), 
								 -( m_pos.y - m_pos.p_y ) * 2 * D3DX_PI / ( gameWindow.height * 10 ),
								 0.0f 
							   );
			}

			player.GetCamera().GetCameraMatrix( view );
			D3DXMATRIX mat[ 7 ];
			D3DXMatrixRotationX( &world_t, fAngle / 2 );
			D3DXMatrixRotationY( &cur,  -fAngle );
			world_t *= cur;
			D3DXMatrixRotationZ( &cur, fAngle );
			world_t *= cur;

			mat[ 0 ] = mat[ 1 ] = mat[ 2 ] = mat[ 3 ] = world_t;
			D3DXMatrixRotationX( &world_t, 0.0f );
			mat[ 4 ] = mat[ 5 ] = mat[ 6 ] = world_t;
			render.BeginDraw();
			level.DrawLevelObject( mat, view );
			D3DXMatrixRotationY( &world,  0.0f );
			D3DXMatrixMultiply( &world, &world, &view );
			level.DrawLevel( world );
			player.DrawView();

			render.EndDraw();
		}
	}

	UnregisterClass( CLASS_NAME, gameWindow.hInstance );

	return 0;

}