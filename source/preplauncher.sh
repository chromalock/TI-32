node ./prepprog.mjs ./programs/LAUNCHER.8xp ./launcher.var
xxd -i ./launcher.var > ./esp32/launcher.h
# certutil -f -encodehex .\launcher.var .\esp32\launcher.h 12
# use this for windows ^ (untested)
