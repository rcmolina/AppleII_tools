rem forfiles /m *.mzf /C "cmd /c mzf2wav @file @fname.wav
forfiles /m *.wav /C "cmd /c wav2tzx @file @fname_id11.tzx
del *.wav
mkdir 2TZX
move *.tzx 2TZX
PAUSE