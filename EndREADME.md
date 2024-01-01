## Aitotal

This program is designed to check files for malware on the service https://www.virustotal.com
The program is able to check both individual files and archives (archive is unpacked into a temporary folder).

This program checks files on VirusTotal.com and displays the result. **This program does not replace antiviruses**.

How to use:
- Drag & drop files/folders on the icon.
- Drag & drop files/folders on a running program.
- Through the "File" menu, select "Open file" and "Open folder".
- Through the command line "Path to program\aitotal.exe" /scan "file name" or "Path to program\aitotal.exe" /norescan "file name" if you don't need to recheck the file.

To stop the checking, press the "Stop" button.

There is an option "Advanced-Settings". Settings are saved to the file "tools/options.ini"

Customizable:
- The number of uploaded files (maximum 20). In the configuration file [Tools] FileCount=20 (not more than 20).
- Total number of files to check (maximum 50). In the configuration file [Tools] ThreadCount=50 (not more than 50)
- Apikey key. apikey=
- The action when closing the program: Close the program, or Minimize in Tray. In the [Tools] Settings file, Exit_Trey= (0-Minimize to Tray, 1-Close program)
- Notification of archive decompression error. In [Tools] MessageErrorArchive=1 configuration file (0 do not print, 1-print)
- Proxy Settings.
- Configure prohibition of duplicate program run, only in the configuration file [Tools] RunDuplicateApplication=0 (no 0-don't create duplicate, however transfer all data to the main form, 1-allows to create duplicate.)
- Output message about the end of file checking [Tools] BallonHint=0 (0 do not print, 1 - print)

Abilities:
- Aitotal automatically unpacks files with popular virus passwords. If the archive is large, the program may hang for the time of unpacking (after the unpacking it will continue normal work).
- Understands the quarantine structure of AVZ, uVS, TDSKiller - that is, you can drag & drop a quarantine archive of such programs, and you will see the file names as they were called initially. AntiSMS quarantines are also supported.
- Saving the verification report to CSV file.
- Applying custom passwords. The file password.txt should contain not more than 10 entries.
- Added password input in the form. If no passwords are applicable, the form for entering the password is displayed.

For unzipping, the 7-zip source code is used, author: Igor Pavlov. Link: http://7-zip.org.ua/ru/

