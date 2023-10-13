let project = new Project("ArchHero");

project.kore = false;

project.addFile("ArchHero/**");

project.setDebugDir("Deployment");

project.addIncludeDir("./TheEngine/includes")

project.flatten();

const otherproj = await project.addProject("./TheEngine");
otherproj.addProvider(project, true);

resolve(project);