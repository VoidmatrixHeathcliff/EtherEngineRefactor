Algorithm       = require("@Algorithm")
Graphic         = require("@Graphic")
Interactivity   = require("@Interactivity")
JSON            = require("@JSON")
Media           = require("@Media")
Network         = require("@Network")
OS              = require("@OS")
String          = require("@String")
Time            = require("@Time")
Window          = require("@Window")
Compress        = require("@Compress")
XML             = require("@XML")

-- DSCompiler = UsingModule("DSCompiler")
-- comp_result = DSCompiler.CompileFile("scripts/Test.ds")
-- JSON.DumpJSONToFile(comp_result, "scripts/AST.json")

-- print(_VERSION_ENGINE)

Window.Create(
    "HelloWorld",
    {
        x = Window.DEFAULT_POS,
        y = Window.DEFAULT_POS,
        w = 1280,
        h = 720
    }
)

isQuit = false

while not isQuit do

    local _start_time = Time.GetInitTime()

    Graphic.SetDrawColor({r = 0, g = 0, b = 0, a = 255})
    Window.Clear()

    while Interactivity.UpdateEvent() do
        local _event = Interactivity.GetEventType()
        if _event == Interactivity.EVENT_QUIT then
            isQuit = true
        end
    end

    Graphic.SetDrawColor({r = 125, g = 200, b = 79, a = 255})
    Graphic.DrawCircle({x = 150, y = 150}, 100, true)

    Window.Update()

    local _end_time = Time.GetInitTime()

    if _end_time > _start_time then
        print(1000 / (_end_time - _start_time))
    end

end

-- font = Graphic.LoadFontFromFile("whz.ttf", 35)
-- image = Graphic.CreateUTF8TextImageBlended(font, "中国智造", 
--     {r = 195, g = 195, b = 195, a = 255})
-- texture = Graphic.CreateTexture(image)
-- width, height = image:GetSize()
-- Graphic.CopyTexture(texture, {x = 0, y = 0, w = width, h = height})
-- Graphic.SetDrawColor({r = 255, g = 255, b = 255, a = 255})
-- Graphic.DrawFillCircle({x = 100, y = 100}, 25)
-- Window.UpdateWindow()

-- while true do Interactivity.UpdateEvent() end

-- os.execute("CHCP 65001")

-- str = "123456789这是中文混搭字符"

-- for i = 1, #str do
--     print(String.SubStrUTF8(str, 1, i))
-- end


-- image = Graphic.LoadImageFromFile("ACG.GY_18.jpg")
-- -- texture = Graphic.CreateTexture(123)  -- 期望接收一个 “Graphic.Image” 类型变量
-- width, height = image:GetSize()
-- print(width, height)
-- image = nil

-- -- 闭包

-- -- if music_type == Media.MUSIC_TYPE_MP3 then
-- if music_type == 1314 then
--     -- 操作
-- end

-- for k, v in pairs(Media) do
--     -- print(k, v)
-- end

-- function test()
--     c{456}
-- end

-- function proxy()
--     test()
-- end

-- proxy()
-- print(debug.traceback())


-- JSON.DumpJSONToFile({a = 10}, String.UTF8ToGBK("测试.json"))

-- while true do
--     doc = XML.LoadXMLFromFile("Hello.xml")
--     node = doc:GetChild("doc"):GetChild("Content")
--     print(node:Print())
--     print(node:GetName())
--     node:SetName("NewContent")
--     print(node:GetName())
--     print(doc:Print(false))
--     break
-- end

-- for name, module in pairs(modules) do
--     print(name..": ")
--     for key, value in pairs(module) do
--         print(key, value)
--     end
--     print("============================")
-- end

-- Window.CreateWindow("TestWindow", {x = Window.WINDOW_POSITION_DEFAULT, y = Window.WINDOW_POSITION_DEFAULT, w = 640, h = 360}, {})
-- Window.CreateWindow(
--     "TestWindow",
--     {
--         x = Window.WINDOW_POSITION_DEFAULT,
--         y = Window.WINDOW_POSITION_DEFAULT,
--         w = 1280,
--         h = 720
--     },
--     {Window.WINDOW_FULLSCREEN_DESKTOP}
-- )
-- local _widthWindowDrawable, _heightWindowDrawable = Window.GetWindowDrawableSize()
-- print(_widthWindowDrawable, _heightWindowDrawable)

-- font = Graphic.LoadFontFromFile("SIMYOU.TTF", 45)
-- font:SetStyle({Graphic.FONT_STYLE_BOLD, Graphic.FONT_STYLE_NORMAL})
-- -- image = Graphic.LoadImageFromFile("test.png")
-- image = Graphic.CreateUTF8TextImageBlended(font, "这是一句字符串……", {r = 255, g = 255, b = 255, a = 255})
-- local _img_width, _img_height = image:GetSize()
-- texture = Graphic.CreateTexture(image)
-- Graphic.CopyTexture(texture, {x = 1530, y = 0, w = _img_width, h = _img_height})
-- -- Graphic.CopyRotateTexture(
-- --     texture,
-- --     0,
-- --     -- {x = _img_width / 2, y = _img_height / 2},
-- --     {x = 320, y = 180},
-- --     {Graphic.FLIP_NONE},
-- --     {x = 0, y = 0, w = _img_width, h = _img_height}
-- -- )
-- Window.UpdateWindow()
-- -- print(String.GBKToUTF8("这是UTF8字符串"))

-- while true do
--     -- image = Graphic.LoadImageFromFile("ACG.GY_18.jpg")
--     Interactivity.UpdateEvent()
-- end

-- while true do
--     while Interactivity.UpdateEvent() do
--         local _event = Interactivity.GetEventType()

--         if _event == Interactivity.EVENT_KEYDOWN_0 then
--             -- print(" 0 Down ")
--         elseif _event == Interactivity.EVENT_WINDOW_MOVE then
--             -- print(" Window Move ")
--         elseif _event == Interactivity.EVENT_QUIT then
--             -- print(" Quit ")
--         elseif _event == Interactivity.EVENT_MOUSEMOTION then
--             -- print(" Motion ")
--         end

--     end

--     print(Interactivity.GetCursorPosition().x, Interactivity.GetCursorPosition().y)
    
--     Window.UpdateWindow()
-- end

-- print("/9j/4AAQSkZJRgABAQEAYABgAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wgARCAAfADcDASIAAhEBAxEB/8QAGQABAQADAQAAAAAAAAAAAAAAAAUBAwYE/8QAFwEBAQEBAAAAAAAAAAAAAAAAAgEAA//aAAwDAQACEAMQAAAB78nclRaSm5H9DNAap1FyWjHoKcrjqzIa/wD/xAAdEAADAQACAwEAAAAAAAAAAAABAgMEETAAEDEy/9oACAEBAAEFAuhvsvzVikV00Ms+udk9MCTLni4JzrKniKXydH//xAAYEQACAwAAAAAAAAAAAAAAAAACEQAQIf/aAAgBAwEBPwERcWqxJR67/8QAGREAAwEBAQAAAAAAAAAAAAAAAgMSABAR/9oACAECAQE/AWsjCXo11irwhIz3/8QAJBAAAQIFBAIDAAAAAAAAAAAAAQIDABESITEEEzBBECM0UdH/2gAIAQEABj8C4lrGUpJhLk2rsblPc/yGwXG95SQSgG/mwMGcOgZpMI9avhU47+o0mnQw4hxBSTNEqZZvw//EACAQAQABAwQDAQAAAAAAAAAAAAERACFRMDFBYRBxkbH/2gAIAQEAAT8h0CATmv1qNSAT0UvMqO3ikVkUYSTtM+QkqMFAAE90TCogObUe+ynfj9dUzhfc7yshnrOj/9oADAMBAAIAAwAAABDnfvyk3/z/xAAeEQACAgEFAQAAAAAAAAAAAAABEQAhEFGBocHR4f/aAAgBAwEBPxB1OFYA0Dr89xZheSpbcAdY/8QAHhEAAgIBBQEAAAAAAAAAAAAAAREAIRBRYaHB0eH/2gAIAQIBAT8QTBTgRFUVkaffMVQSlBWo7e75J7x//8QAIBABAQACAgAHAAAAAAAAAAAAAREAIRBBIDAxYXHB8P/aAAgBAQABPxDyA0jVmKr99DBZQENKiX21hVBRuYNL+Nts3gI9b4FywI3fXMlCqomDhKICYRdcqpcA7c1E7Bo7GvuzVLKpSkEFIlbobnABYBdvi//Z")

-- str_base64 = Algorithm.EncodeBase64(io.open("123.jpg", "rb"):read("*a"))

-- img_data = Algorithm.DecodeBase64(str_base64)

-- io.open("456.jpg", "wb"):write(img_data)

-- res = Network.CreateClient("https://i.pixiv.cat"):Get("/img-original/img/2021/03/01/16/13/06/88140305_p1.png")
-- file = io.open("r18.png", "wb")
-- print(res.body)
-- file:write(res.body)
-- file:flush()
-- file:close()
-- print("status：", res.status)
-- for k, v in pairs(res.headers) do
--     print(k, v)
-- end
-- domain, route, param = Network.SplitLink("https://www.baidu.com/index?user=xiaominng&age=19")
-- print(domain, route, param)

-- file = io.open("SIMYOU.TTF", "rb")

-- data_compressed = Compress.CompressData(file:read("*a"), 5) 

-- file_output_compressed = io.open("compressed.jpgc", "wb")

-- file_output_compressed:write(data_compressed)

-- data_decompressed = Compress.DecompressData(data_compressed) 

-- file_output_decompressed = io.open("decompressed.jpg", "wb")

-- file_output_decompressed:write(data_decompressed)

-- file = io.open("ACG.GY_18.jpg", "rb")
-- print(file:seek("end"))

-- file_output_decompressed = io.open("decompressed.jpg", "rb")
-- print(file_output_decompressed:seek("end"))


-- Network = UsingModule("Network")

-- server = Network.CreateServer()

-- file = io.open("ACG.GY_18.jpg", "rb")

-- server:Get("/hi", function(req, res)
--     file:seek("set")
--     res.SetContent(file:read("*a"), "image/jpeg")
-- end)

-- server:Get("/stop", function(req, res)
--     server:Stop()
-- end)

-- server:SetExceptionHandler(function(req, res, err) print("err: "..err) end)

-- server:Listen("localhost", 1234)


-- file = io.open("ACG.GY_18.jpg", "rb")
-- Window.CreateWindow("TestWindow", {x = Window.WINDOW_POSITION_DEFAULT, y = Window.WINDOW_POSITION_DEFAULT, w = 640, h = 360}, {})
-- image = Graphic.LoadImageFromData(file:read("*a"))
-- texture = Graphic.CreateTexture(image)
-- Graphic.CopyTexture(texture, {x = 0, y = 0, w = 1280, h = 720})
-- Window.UpdateWindow()

-- while true do
--     file:seek("set")
--     image = Graphic.LoadImageFromData(file:read("*a"))
--     Interactivity.UpdateEvent()
-- end

