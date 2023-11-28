
tell application "QuickTime Player"
    new screen recording
    delay 5
    -- press record button
end tell

tell application "System Events"
    -- stop the recording
    key code 53 using {command down, control down}
    
    -- wait for recorded window to appear
    delay 5
    
    keystroke "s" using command down
end tell
