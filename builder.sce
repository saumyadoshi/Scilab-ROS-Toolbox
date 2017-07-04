files = ["ros_toolbox.c"];
ilib_build('build_lib', ['ros', 'ros_toolbox'], files, []);
exec loader.sce
