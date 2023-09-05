const fs = require('fs');
const path = require('path');
let project = new Project('engine');
 
project.addProvider = function(proj, isRoot=false){
    const sdl2 = true;//process.argv.indexOf("--sdl2") >= 0;
    if(sdl2){
        
        if(!isRoot){
            fs.copyFileSync("./sdl/lib/SDL2.dll", "./Deployment/SDL2.dll");
            proj.addDefine("USE_SDL");
            proj.addIncludeDir(path.resolve("./sdl/include"));
        }
        proj.addLib("./sdl/lib/SDL2");
        proj.addLib("./sdl/lib/SDL2main");
    }
};
project.kore = false;


project.addDefine("KINC_STATIC_COMPILE");
project.isStaticLib = true;

project.addIncludeDir(path.resolve("./TheEngine/includes"));
project.addFiles('src/**','includes/**');

project.addProvider(project,false);

resolve(project);