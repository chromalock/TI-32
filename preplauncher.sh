node ./prepare8xp.mjs ./programs/LAUNCHER.8xp ./launcher.var
xxd -i ./launcher.var > ./esp32/launcher.h