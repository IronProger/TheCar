#! /usr/bin/lua

--[ for color detect (HSV) ]--

-- for red road signs
-- 0…179 (Hue)
RED_I_LOW_H = 0
RED_I_HIGH_H = 40
-- 0…255 (Saturation)
RED_I_LOW_S = 140
RED_I_HIGH_S = 255
-- 0…255 (Value)
RED_I_LOW_V = 160
RED_I_HIGH_V = 255

-- for blue road signs
-- 0…179 (Hue)
BLUE_I_LOW_H = 100
BLUE_I_HIGH_H = 179
-- 0…255 (Saturation)
BLUE_I_LOW_S = 160
BLUE_I_HIGH_S = 255
-- 0…255 (Value)
BLUE_I_LOW_V = 160
BLUE_I_HIGH_V = 255


--[ GUI ]--

SHOW_WINDOWS = true


--[ video ]--

VIDEO_INPUT_SOURCE_NUMBER = 0

--[ Hough circle transform parametrs ]--

H_DP = 24
H_MINDIST = 25
H_PARAM1 = 200
H_PARAM2 = 100
H_MINRADIUS = 25
H_MAXRADIUS = 160

--[ other ]--

EDGE_THRESHOLD = 16
FOLDER_FOR_TEST_IMAGES_OUTPUT = "test_images_output"

--[ connection with robot ]--

SERIAL_PORT = "/dev/ttyACM0"
FAKE_SERIAL = true -- replace real serial with stdout

--[ debug ]--

SAVE_IMAGES = true