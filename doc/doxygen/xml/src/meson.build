tgt = custom_target(
	'xml-Server-doc',
	command: [
		gen_doxygen,
		# XXX pass doxygen path as argument
		'--builddir=@OUTDIR@',
		'--section=Server',
		'--output-format=xml',
                wlscenegraph_doxygen,
		'@INPUT@',
	],
	input: [ src_files ],
	output: [ 'combine.xslt', 'index.xml' ],
	depends: [dot_png, dot_map]
)

doxygen_Server_combine_xslt = tgt[0]
doxygen_Server_index_xml = tgt[1]
