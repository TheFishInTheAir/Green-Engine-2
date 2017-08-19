FILENAME_LENGTH = 9
BUILD_FOLDER = ".build"


is_unix = package.config:sub(1,1)=="/"
file_path  = arg[0]
file_dir   = file_path:sub(0, file_path:len()-FILENAME_LENGTH)
build_dir  = file_dir .. BUILD_FOLDER


print("file path:  " .. file_path)
print("file  dir:  " .. file_dir)
print("build dir:  " .. build_dir)

if(arg[1] == "remove") then

	--os.execute("")  --- delete build directory
	
	os.exit()

end

os.execute("mkdir \"" .. build_dir .. "\"") --- create build folder

if(package.config:sub(1,1)=="/") then --- If Unix

os.execute("cmake \"" .. file_dir .. "\" -G \"Unix Makefiles\"") --- TODO: Finish


else ---If Windows

os.execute("cd \"" .. build_dir .. "\" &" ..
"cmake .. -G \"MinGW Makefiles\" &" ..
"mingw32-make -Wextra &" ..
"") --- TODO: Add command to execute program.


end