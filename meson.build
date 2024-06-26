project(
	'wlscenegraph',
	'c',
	version: '0.1.0-dev',
	license: 'MIT',
	meson_version: '>=0.59.0',
	default_options: [
		'c_std=c11',
		'warning_level=2',
		'werror=true',
	],
)

version = meson.project_version().split('-')[0]
version_major = version.split('.')[0]
version_minor = version.split('.')[1]
assert(version_major == '0')
soversion = version_minor.to_int() - 5

little_endian = target_machine.endian() == 'little'
big_endian = target_machine.endian() == 'big'

add_project_arguments([
	'-D_POSIX_C_SOURCE=200809L',
	'-DWLR_USE_UNSTABLE',
	'-DWLR_LITTLE_ENDIAN=@0@'.format(little_endian.to_int()),
	'-DWLR_BIG_ENDIAN=@0@'.format(big_endian.to_int()),
], language: 'c')

cc = meson.get_compiler('c')

add_project_arguments(cc.get_supported_arguments([
	'-Wundef',
	'-Wlogical-op',
	'-Wmissing-include-dirs',
	'-Wold-style-definition',
	'-Wpointer-arith',
	'-Winit-self',
	'-Wstrict-prototypes',
	'-Wimplicit-fallthrough=2',
	'-Wendif-labels',
	'-Wstrict-aliasing=2',
	'-Woverflow',
	'-Wmissing-prototypes',
	'-Walloca',

	'-Wno-missing-braces',
	'-Wno-missing-field-initializers',
	'-Wno-unused-parameter',
]), language: 'c')

# Compute the relative path used by compiler invocations.
source_root = meson.current_source_dir().split('/')
build_root = meson.global_build_root().split('/')
relative_dir_parts = []
i = 0
in_prefix = true
foreach p : build_root
	if i >= source_root.length() or not in_prefix or p != source_root[i]
		in_prefix = false
		relative_dir_parts += '..'
	endif
	i += 1
endforeach
i = 0
in_prefix = true
foreach p : source_root
	if i >= build_root.length() or not in_prefix or build_root[i] != p
		in_prefix = false
		relative_dir_parts += p
	endif
	i += 1
endforeach
relative_dir = join_paths(relative_dir_parts) + '/'

# Strip relative path prefixes from the code if possible, otherwise hide them.
if cc.has_argument('-fmacro-prefix-map=/prefix/to/hide=')
	add_project_arguments(
		'-fmacro-prefix-map=@0@='.format(relative_dir),
		language: 'c',
	)
else
	add_project_arguments(
		'-D_WLR_REL_SRC_DIR="@0@"'.format(relative_dir),
		language: 'c',
	)
endif

features = {
	
}

internal_features = {

}

internal_config = configuration_data()

wayland_project_options = ['tests=false', 'documentation=false']
xkbcommon = dependency(
	'xkbcommon',
	fallback: 'libxkbcommon',
	default_options: [
		'enable-tools=false',
		'enable-x11=false',
		'enable-docs=false',
		'enable-xkbregistry=false',
	],
)
math = cc.find_library('m')
rt = cc.find_library('rt')
wlroots_version = ['>=0.17.0', '<0.18.0']
wlroots = dependency('wlroots', version: wlroots_version)
wayland_server = dependency('wayland-server', version: '>=1.20.0')

wls_files = []
wls_deps = [
	math,
	rt,
    xkbcommon,
    wlroots,
    wayland_server,
]

subdir('protocol')
subdir('include')
subdir('backend')
wls_inc = include_directories('include')

if get_option('documentation').enabled()
        subdir('doc')
endif

symbols_file = 'wlscenegraph.syms'
symbols_flag = '-Wl,--version-script,@0@/@1@'.format(meson.current_source_dir(), symbols_file)
lib_wls = library(
	meson.project_name(), wls_files,
	soversion: soversion.to_string(),
	dependencies: wls_deps,
	include_directories: [wls_inc],
	install: true,
	link_args: symbols_flag,
	link_depends: symbols_file,
)

wls_vars = {}
foreach name, have : features
	wls_vars += { 'have_' + name.underscorify(): have.to_string() }
endforeach

wlscenegraph = declare_dependency(
	link_with: lib_wls,
	dependencies: wls_deps,
	include_directories: wls_inc,
	variables: wls_vars,
)

meson.override_dependency('wlscenegraph', wlscenegraph)

summary(features + internal_features, bool_yn: true)

subdir('tinywl')

pkgconfig = import('pkgconfig')
pkgconfig.generate(
	lib_wls,
	description: 'open source high performance graphics toolkit for Wayland compositor',
	url: 'https://github.com/zzxyb/wlscenegraph',
	variables: wls_vars,
)
