const fs = require('fs');
const path = require('path');
let project = new Project('engine');
 
project.addProvider = function(proj, isRoot=false){
    proj.addIncludeDir("C:/Program Files (x86)/Visual Leak Detector/include")
    proj.addLib("C:/Program Files (x86)/Visual Leak Detector/lib/Win64/vld")
    const sdl2 = true;//process.argv.indexOf("--sdl2") >= 0;
    if(sdl2){
        if(!isRoot){
            fs.copyFileSync("./sdl/lib/SDL2.dll", "./Deployment/SDL2.dll");
            fs.copyFileSync("./sdl/lib/SDL2_image.dll", "./Deployment/SDL2_image.dll");
            fs.copyFileSync("./sdl/lib/SDL2_mixer.dll", "./Deployment/SDL2_mixer.dll");
            fs.copyFileSync("./sdl/lib/SDL2_ttf.dll", "./Deployment/SDL2_ttf.dll");
            proj.addDefine("USE_SDL");
            proj.addIncludeDir(path.resolve("./sdl/include"));
        }
        proj.addLib("./sdl/lib/SDL2");
        proj.addLib("./sdl/lib/SDL2main");
        proj.addLib("./sdl/lib/SDL2_image");
        proj.addLib("./sdl/lib/SDL2_mixer");
        proj.addLib("./sdl/lib/SDL2_ttf");
    }
};
project.kore = false;
project.setCppStd("c++17");


project.addDefine("KINC_STATIC_COMPILE");
project.isStaticLib = true;

project.addIncludeDir(path.resolve("./TheEngine/includes"));
project.addFiles('src/**','includes/**');

project.addProvider(project,false);

resolve(project);