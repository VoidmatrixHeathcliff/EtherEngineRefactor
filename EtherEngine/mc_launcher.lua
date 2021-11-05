UsingModule("All")

rectWindow = {
    x = Window.WINDOW_POSITION_DEFAULT,
    y = Window.WINDOW_POSITION_DEFAULT, 
    w = 640, 
    h = 360
}

CreateWindow("我的世界启动器", rectWindow, {})

SetWindowIcon(LoadImage("Creeper.ico"))

StartTextInput()

userName = "_"

fontUserName = LoadFont("SIMYOU.TTF", 45)
fontBtnText = LoadFont("SIMYOU.TTF", 35)

while true do

    local _timeStart = GetInitTime()

    SetDrawColor({r = 200, g = 200, b = 200, a = 255})
    ClearWindow()

    if UpdateEvent() then
        local _event = GetEventType()
        if _event == Interactivity.EVENT_QUIT then
            break
        elseif _event == Interactivity.EVENT_MOUSEBTNUP_LEFT then
            if userName == "_" then
                ShowMessageBox("启动失败", "玩家名不可为空", Window.MSGBOX_WARNING)
            else
                os.execute("BootMC.bat")
                break
            end
        elseif _event == Interactivity.EVENT_TEXTINPUT then
            if userName == "_" then userName = "" end
            userName = userName..GetInputText()
        elseif _event == Interactivity.EVENT_KEYDOWN_BACKSPACE then
            userName = string.sub(userName, 1, #userName - 1)
            if userName == "" then userName = "_" end
        end
    end

    local _imageUserName = CreateUTF8TextImageBlended(
        fontUserName,
        userName,
        {r = 45, g = 45, b = 45, a = 255}
    )
    local _textureUserName = CreateTexture(_imageUserName)
    local _widthImageUserName, _heightImageUsername = GetImageSize(_imageUserName)
    CopyTexture(
        _textureUserName,
        {
            x = rectWindow.w / 2 - _widthImageUserName / 2,
            y = 100,
            w = _widthImageUserName,
            h = _heightImageUsername
        }
    )
    DestroyTexture(_textureUserName)
    UnloadImage(_imageUserName)

    local _imageBtnText = CreateUTF8TextImageBlended(
        fontBtnText,
        "启动游戏",
        {r = 235, g = 235, b = 235, a = 255}
    )
    local _textureBtnText = CreateTexture(_imageBtnText)
    local _widthImageBtnText, _heightImageBtnText = GetImageSize(_imageBtnText)
    local _rectBtnText = {
        x = rectWindow.w / 2 - _widthImageBtnText / 2,
        y = 200,
        w = _widthImageBtnText,
        h = _heightImageBtnText
    }
    local _rectBtnBorder = {
        x = rectWindow.w / 2 - _rectBtnText.w / 2 - 10,
        y = _rectBtnText.y - 8,
        w = _rectBtnText.w + 20,
        h = _rectBtnText.h + 16
    }
    SetDrawColor({r = 57, g = 62, b = 79, a = 255})
    FillRoundRectangle(_rectBtnBorder, 5)
    SetDrawColor({r = 85, g = 41, b = 91, a = 255})
    RoundRectangle(_rectBtnBorder ,5)
    CopyTexture(_textureBtnText, _rectBtnText)
    DestroyTexture(_textureBtnText)
    UnloadImage(_imageBtnText)

    UpdateWindow()

    local _timeEnd = GetInitTime()

    DynamicSleep(1000 / 60, _timeEnd - _timeStart)
end

StopTextInput()