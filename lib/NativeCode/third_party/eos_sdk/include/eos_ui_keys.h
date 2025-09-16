// Copyright Epic Games, Inc. All Rights Reserved.

// This file is not intended to be included directly. Include eos_ui_types.h instead.

/** Number of bits to shift the modifiers into the integer. */
EOS_UI_KEY_CONSTANT(EOS_UIK_, ModifierShift, 16)
/** A mask to isolate the single key. */
EOS_UI_KEY_CONSTANT(EOS_UIK_, KeyTypeMask, (1 << EOS_UIK_ModifierShift) - 1)
/** A mask to isolate the modifier keys. */
EOS_UI_KEY_CONSTANT(EOS_UIK_, ModifierMask, ~EOS_UIK_KeyTypeMask)

/** The Shift key */
EOS_UI_KEY_MODIFIER(EOS_UIK_, Shift, (1 << EOS_UIK_ModifierShift))
/** The Control key */
EOS_UI_KEY_MODIFIER(EOS_UIK_, Control, (2 << EOS_UIK_ModifierShift))
/** The Alt key */
EOS_UI_KEY_MODIFIER(EOS_UIK_, Alt, (4 << EOS_UIK_ModifierShift))
/** The Windows key on a Windows keyboard or the Command key on a Mac keyboard */
EOS_UI_KEY_MODIFIER(EOS_UIK_, Meta, (8 << EOS_UIK_ModifierShift))
/** A mask which contains all of the modifier keys */
EOS_UI_KEY_CONSTANT(EOS_UIK_, ValidModifierMask, (EOS_UIK_Shift | EOS_UIK_Control | EOS_UIK_Alt | EOS_UIK_Meta))

/** The default value, not assigned to a key */
EOS_UI_KEY_ENTRY_FIRST(EOS_UIK_, None, 0)
/** The Space key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Space)
/** The Backspace key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Backspace)
/** The Tab key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Tab)
/** The Escape key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Escape)

/** The PageUp key */
EOS_UI_KEY_ENTRY(EOS_UIK_, PageUp)
/** The PageDown key */
EOS_UI_KEY_ENTRY(EOS_UIK_, PageDown)
/** The End key */
EOS_UI_KEY_ENTRY(EOS_UIK_, End)
/** The Home key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Home)
/** The Insert key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Insert)
/** The Delete key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Delete)

/** The Left Arrow key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Left)
/** The Up Arrow key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Up)
/** The Right Arrow key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Right)
/** The Down Arrow key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Down)

/** The 0 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key0)
/** The 1 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key1)
/** The 2 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key2)
/** The 3 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key3)
/** The 4 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key4)
/** The 5 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key5)
/** The 6 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key6)
/** The 7 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key7)
/** The 8 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key8)
/** The 9 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Key9)

/** The A key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyA)
/** The B key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyB)
/** The C key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyC)
/** The D key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyD)
/** The E key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyE)
/** The F key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyF)
/** The G key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyG)
/** The H key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyH)
/** The I key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyI)
/** The J key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyJ)
/** The K key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyK)
/** The L key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyL)
/** The M key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyM)
/** The N key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyN)
/** The O key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyO)
/** The P key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyP)
/** The Q key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyQ)
/** The R key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyR)
/** The S key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyS)
/** The T key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyT)
/** The U key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyU)
/** The V key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyV)
/** The W key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyW)
/** The X key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyX)
/** The Y key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyY)
/** The Z key */
EOS_UI_KEY_ENTRY(EOS_UIK_, KeyZ)

/** The Numpad 0 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad0)
/** The Numpad 1 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad1)
/** The Numpad 2 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad2)
/** The Numpad 3 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad3)
/** The Numpad 4 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad4)
/** The Numpad 5 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad5)
/** The Numpad 6 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad6)
/** The Numpad 7 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad7)
/** The Numpad 8 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad8)
/** The Numpad 9 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, Numpad9)
/** The Numpad '*' key */
EOS_UI_KEY_ENTRY(EOS_UIK_, NumpadAsterisk)
/** The Numpad '+' key */
EOS_UI_KEY_ENTRY(EOS_UIK_, NumpadPlus)
/** The Numpad '-' key */
EOS_UI_KEY_ENTRY(EOS_UIK_, NumpadMinus)
/** The Numpad '.' key */
EOS_UI_KEY_ENTRY(EOS_UIK_, NumpadPeriod)
/** The Numpad '/' key */
EOS_UI_KEY_ENTRY(EOS_UIK_, NumpadDivide)

/** The F1 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F1)
/** The F2 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F2)
/** The F3 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F3)
/** The F4 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F4)
/** The F5 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F5)
/** The F6 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F6)
/** The F7 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F7)
/** The F8 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F8)
/** The F9 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F9)
/** The F10 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F10)
/** The F11 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F11)
/** The F12 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F12)
/** The F13 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F13)
/** The F14 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F14)
/** The F15 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F15)
/** The F16 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F16)
/** The F17 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F17)
/** The F18 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F18)
/** The F19 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F19)
/** The F20 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F20)
/** The F21 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F21)
/** The F22 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F22)
/** The F23 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F23)
/** The F24 key */
EOS_UI_KEY_ENTRY(EOS_UIK_, F24)

/** '+' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, OemPlus)
/** ',' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, OemComma)
/** '-' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, OemMinus)
/** '.' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, OemPeriod)
/** ';' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem1)
/** '/' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem2)
/** '~' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem3)
/** '[' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem4)
/** '\' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem5)
/** ']' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem6)
/** '"' for US layout, others vary */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem7)
/** Varies on all layouts */
EOS_UI_KEY_ENTRY(EOS_UIK_, Oem8)

/** Maximum key enumeration value. */
EOS_UI_KEY_CONSTANT_LAST(EOS_UIK_, MaxKeyType)
