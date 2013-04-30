#include "lib/universal_include.h"

#include <time.h>
#include <stdlib.h>

#include "lib/debug_utils.h"
#include "lib/hi_res_time.h"
#include "lib/input.h"
#include "lib/window_manager_gdi.h"

#include "app.h"


#define LMB_REPEAT_PERIOD			0.1f	// Seconds


InputManager *g_inputManager = NULL;
signed char g_keyDowns[KEY_MAX];
signed char g_keyUps[KEY_MAX];
signed char g_keys[KEY_MAX];
  

InputManager::InputManager()
:   m_windowHasFocus(true),
    m_windowClosed(false),
    m_lmb(false),
    m_lmbPrivate(false),
    m_mmb(false),
    m_rmb(false),
    m_lmbOld(false),
    m_mmbOld(false),
    m_rmbOld(false),
	m_lastClickWasNC(false),
	m_lastClickTime(0.0),
	m_lmbRepeatsSoFar(0),
	m_rawLmbClicked(false),
	m_lmbDoubleClicked(false),
    m_lmbClicked(false),
    m_mmbClicked(false),
    m_rmbClicked(false),
    m_lmbUnClicked(false),
    m_mmbUnClicked(false),
    m_rmbUnClicked(false),
    m_mouseX(0),
    m_mouseY(0),
    m_mouseZ(0),
    m_mouseOldX(0),
    m_mouseOldY(0),
    m_mouseOldZ(0),
    m_mouseVelX(0),
    m_mouseVelY(0),
    m_mouseVelZ(0),
	m_numKeysTyped(0),
	m_newNumKeysTyped(0),
	m_mouseCallback(NULL)
{
	memset(g_keys, 0, KEY_MAX);
	memset(g_keyDowns, 0, KEY_MAX);
	memset(g_keyUps, 0, KEY_MAX);
	memset(m_keyNewDowns, 0, KEY_MAX);
	memset(m_keyNewUps, 0, KEY_MAX);
}


// Returns 0 if the event is handled here, -1 otherwise
int InputManager::EventHandler(unsigned int message, unsigned int wParam, int lParam, bool _isAReplayedEvent)
{
	switch (message) 
	{
		case WM_SETFOCUS:
		case WM_NCACTIVATE:
			m_windowHasFocus = true;
			// Clear keyboard state when we regain focus
			memset(m_keyNewDowns, 0, KEY_MAX);
			memset(m_keyNewUps, 0, KEY_MAX);
			memset(g_keys, 0, KEY_MAX);
			return -1;
		case WM_KILLFOCUS:
			m_windowHasFocus = false;
            m_mouseX = -1;
            m_mouseY = -1;
			break;

        case WM_SYSCHAR:
            break;

		case WM_CHAR:
			if (m_numKeysTyped < MAX_KEYS_TYPED_PER_FRAME &&
				!g_keys[KEY_CONTROL] &&
				!g_keys[KEY_ALT] &&
				wParam != KEY_ESC) 
			{
				ReleaseAssert(wParam < KEY_MAX, 
					"Keypress value out of range (WM_CHAR: wParam = %d)", wParam);
				m_newKeysTyped[m_newNumKeysTyped] = wParam;
				m_newNumKeysTyped++;
			}
			break;

		case WM_LBUTTONDOWN:
			m_lmbPrivate = true;
			g_windowManager->CaptureMouse();
			break;
		case WM_LBUTTONUP:
			m_lmbPrivate = false;
			g_windowManager->UncaptureMouse();
			break;
		case WM_MBUTTONDOWN:
			m_mmb = true;
			g_windowManager->CaptureMouse();
			break;
		case WM_MBUTTONUP:
			m_mmb = false;
			g_windowManager->UncaptureMouse();
			break;
		case WM_RBUTTONDOWN:
			m_rmb = true;
			g_windowManager->CaptureMouse();
			break;
		case WM_RBUTTONUP:
			m_rmb = false;
			g_windowManager->UncaptureMouse();
			break;

		/* Mouse clicks in the Non-client area (ie titlebar) of the window are weird. If we
		   handle them and return 0, then Windows ignores them and it is no longer possible
		   to drag the window by the title bar. If we handle them and return -1, then Windows
		   never sends us the button up event. I've chosen the second option and fix some of
		   the brokenness by generating a fake up event one frame after the down event. */
		case WM_NCLBUTTONDOWN:
			m_lmbPrivate = true;
			m_lastClickWasNC = true;
			return -1;
			break;
		case WM_NCMBUTTONDOWN:
			m_mmb = true;
			m_lastClickWasNC = true;
			return -1;
			break;
		case WM_NCRBUTTONDOWN:
			m_rmb = true;
			m_lastClickWasNC = true;
			return -1;
			break;

		case 0x020A:
//		case WM_MOUSEWHEEL:
		{
			int move = (short)HIWORD(wParam) / 120;
			m_mouseZ += move;
			break;
		}
		
		case WM_NCMOUSEMOVE:
			m_mouseX = -1;
			m_mouseY = -1;
			break;
				
		case WM_MOUSEMOVE:
		{
			short newPosX = lParam & 0xFFFF;
			short newPosY = short(lParam >> 16);
			m_mouseX = newPosX;
			m_mouseY = newPosY;

			if (m_mouseCallback)
			{
				m_mouseCallback(newPosX, newPosY);			
			}
			break;
		}

		case WM_SYSKEYUP:
			ReleaseAssert(wParam < KEY_MAX, 
				"Keypress value out of range (WM_SYSKEYUP: wParam = %d)", wParam);
            
            // If the key event is the Control part of Alt_Gr, throw it away. There
            // will be an event for the alt part too.
            if (wParam == KEY_CONTROL && GetAsyncKeyState(VK_MENU) < 0)
                break;

            g_keys[wParam] = 0;
			m_keyNewUps[wParam] = 1;
			break;
			
		case WM_SYSKEYDOWN:
		{
			ReleaseAssert(wParam < KEY_MAX, 
				"Keypress value out of range (WM_SYSKEYDOWN: wParam = %d)", wParam);
			int flags = (short)HIWORD(lParam);
			g_keys[wParam] = 1;
			m_keyNewDowns[wParam] = 1;
			break;
		}

		case WM_KEYUP:
		{
			// Alt key ups are presented here when the user keys, for example, Alt+F.
			// Windows will generate a SYSKEYUP event for the release of F, and a
			// normal KEYUP event for the release of the ALT. Very strange.
			ReleaseAssert(wParam < KEY_MAX, 
				"Keypress value out of range (WM_KEYUP: wParam = %d)", wParam);
			g_keys[wParam] = 0;
			m_keyNewUps[wParam] = 1;
			break;
		}

		case WM_KEYDOWN:
		{
			ReleaseAssert(wParam < KEY_MAX, 
				"Keypress value out of range (WM_KEYDOWN: wParam = %d)", wParam);
			if (wParam == KEY_DEL)
			{
				m_newKeysTyped[m_numKeysTyped] = 127;
				m_newNumKeysTyped++;
			}
            else if (wParam == KEY_CONTROL && GetAsyncKeyState(VK_MENU) < 0)
            {
                int res = GetAsyncKeyState(VK_MENU);
                // Key event is the Control part of Alt_Gr, so throw it away. There
                // will be an event for the alt part too.
                break;
            }
			m_keyNewDowns[wParam] = 1;
			g_keys[wParam] = 1;
			break;
		}

		default:
			return -1;
	}

	return 0;
}


void InputManager::Advance()
{
	memcpy(g_keyDowns, m_keyNewDowns, KEY_MAX);
	memcpy(g_keyUps, m_keyNewUps, KEY_MAX);
	memset(m_keyNewDowns, 0, KEY_MAX);
	memset(m_keyNewUps, 0, KEY_MAX);

	m_numKeysTyped = m_newNumKeysTyped;
	memcpy(m_keysTyped, m_newKeysTyped, m_newNumKeysTyped);
	m_newNumKeysTyped = 0;


	// Count the number of key ups and downs this frame
	m_numKeyDowns = 0;
	m_numKeyUps = 0;
	for (int i = 0; i < KEY_MAX; ++i)
	{
		if (g_keyUps[i]) m_numKeyUps++;
		if (g_keyDowns[i]) m_numKeyDowns++;
	}
	
    m_lmb = m_lmbPrivate;
	m_lmbClicked = m_lmb == true && m_lmbOld == false;
	m_mmbClicked = m_mmb == true && m_mmbOld == false;
	m_rmbClicked = m_rmb == true && m_rmbOld == false;
	m_rawLmbClicked = m_lmbClicked;
	m_lmbUnClicked = m_lmb == false && m_lmbOld == true;
	m_mmbUnClicked = m_mmb == false && m_mmbOld == true;
	m_rmbUnClicked = m_rmb == false && m_rmbOld == true;
	m_lmbOld = m_lmb;
	m_mmbOld = m_mmb;
	m_rmbOld = m_rmb;

	m_lmbDoubleClicked = false;
	if (m_lmbClicked)
	{
 		double timeNow = GetHighResTime();
		double delta = timeNow - m_lastClickTime;
		if (delta < 0.25)
		{
			m_lmbDoubleClicked = true;
			m_lmbClicked = false;
		}
		m_lastClickTime = timeNow;
		m_lmbRepeatsSoFar = 0;
	}

	// Generate repeats when the lmb is held
	m_lmbRepeated = false;
	if (m_lmb)
	{
		float timeSinceClick = (GetHighResTime() - m_lastClickTime);
		int maxPossibleRepeats = (int)(timeSinceClick / LMB_REPEAT_PERIOD);
		if (maxPossibleRepeats > 3 && maxPossibleRepeats > m_lmbRepeatsSoFar)
		{
			m_lmbRepeatsSoFar = maxPossibleRepeats;
			m_lmbRepeated = true;
		}
	}
			
	// Generate fake mouse up event(s) to compensate for the fact that Windows won't send
	// them if the last mouse down event was in the client area.
	if (m_lastClickWasNC)
	{
		m_lastClickWasNC = false;
		m_lmbPrivate = false;
		m_mmb = false;
		m_rmb = false;
	}

	m_mouseVelX = m_mouseX - m_mouseOldX;
	m_mouseVelY = m_mouseY - m_mouseOldY;
	m_mouseVelZ = m_mouseZ - m_mouseOldZ;
	m_mouseOldX = m_mouseX;
	m_mouseOldY = m_mouseY;
	m_mouseOldZ = m_mouseZ;
}


void InputManager::PollForMessages()
{
	g_windowManager->NastyPollForMessages();
}


char const *InputManager::GetKeyName(int i)
{
	static char name[16];
	name[0] = '\0';

	if (isdigit(i) || (isalpha(i) && isupper(i)))
	{
		name[0] = i;
		name[1] = '\0';
		return name;
	}

	// Keypad numbers
	if (i >= 96 && i <= 105)
	{
		strcat(name, "Keypad ");
		name[6] = i - 48;
		name[7] = '\0';
		return name;
	}

	// F1 through F10
	if (i >= 112 && i <= 123)
	{
		name[0] = 'F';
		if (i <= 120)
		{
			name[1] = i - 63,
			name[2] = '\0';
		}
		else
		{
			name[1] = '1';
			name[2] = i - 73;
			name[3] = '\0';
		}

		return name;
	}

	// All other named keys
	switch(i)
	{
		case 8:   return "Backspace";         
		case 9:   return "Tab";       
		case 13:  return "Enter";             
		case 16:  return "Shift";		  
		case 17:  return "Ctrl";			  
		case 18:  return "Alt";           
		case 19:  return "Pause";        
		case 20:  return "Capslock";        
		case 27:  return "Esc";           
		case 32:  return "Space";            
		case 33:  return "PgUp";            
		case 34:  return "PgDn";             
		case 35:  return "End";            
		case 36:  return "Home";            
		case 37:  return "Left";              
		case 38:  return "Up";           
		case 39:  return "Right";            
		case 40:  return "Down";          
		case 45:  return "Insert";            
		case 46:  return "Del";               
		case 106: return "*"; 
		case 107: return "Keypad +";         
		case 109: return "Keypad -";      
		case 110: return "Keypad Del";      
		case 111: return "Keypad Slash";
		case 144: return "NumLock";        
		case 145: return "ScrLock";           
		case 186: return ":";             
		case 187: return "=";            
		case 188: return ",";            
		case 189: return "-";             
		case 190: return ".";              
		case 191: return "/";             
		case 192: return "\"";             
		case 219: return "[";         
		case 220: return "\\";         
		case 221: return "]";       
		case 223: return "~";             
	}

	// Unnamed keys just use the ASCII value, printed in decimal
	sprintf(name, "unnamed%d", i);
	return name;
}


int InputManager::GetKeyId(char const *name)
{
	for (int i = 0; i < KEY_MAX; ++i)
	{
		char const *keyName = GetKeyName(i);
		if (keyName && stricmp(name, keyName) == 0)
		{
			return i;
		}
	}

	if (strncmp("unnamed", name, 7) == 0)
	{
		return atoi(name + 7);
	}

	return -1;
}


bool InputManager::UntypeKey(char key)
{
	for (int i = 0; i < m_numKeysTyped; ++i)
	{
		if (m_keysTyped[i] == key)
		{
			m_keysTyped[i] = m_keysTyped[m_numKeysTyped - 1];
			m_numKeysTyped--;
			return true;
		}
	}

	return false;
}
