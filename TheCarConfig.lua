#! /usr/bin/lua

--[ for color detect (HSV) ]--

-- for red road signs
-- 0…179 (Hue)
RED_I_LOW_H = 0
RED_I_HIGH_H = 20
-- 0…255 (Saturation)
RED_I_LOW_S = 120
RED_I_HIGH_S = 255
-- 0…255 (Value)
RED_I_LOW_V = 160
RED_I_HIGH_V = 255

-- for blue road signs
-- 0…179 (Hue)
BLUE_I_LOW_H = 100
BLUE_I_HIGH_H = 160
-- 0…255 (Saturation)
BLUE_I_LOW_S = 170
BLUE_I_HIGH_S = 255
-- 0…255 (Value)
BLUE_I_LOW_V = 170
BLUE_I_HIGH_V = 255


--[ GUI ]--

SHOW_WINDOWS = false


--[ video ]--

VIDEO_INPUT_SOURCE_NUMBER = 1

--[ Hough circle transform parametrs ]--

H_DP = 10
H_MINDIST = 10
H_PARAM1 = 300
H_PARAM2 = 300
H_MINRADIUS = 10
H_MAXRADIUS = 150

--[ other ]--

EDGE_TRASHHOLD = 20