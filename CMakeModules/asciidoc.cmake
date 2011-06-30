find_program(xmlto_exe NAMES xmlto)
find_program(asciidoc_exe NAMES asciidoc)

add_custom_target(doc-man)
add_custom_target(doc-html)

macro (asciidoc_doc manpage section asciidoc_opts xmlto_opts)
    set(invar_txt   ${CMAKE_CURRENT_SOURCE_DIR}/${manpage}.txt)
    set(outvar_xml  ${CMAKE_CURRENT_BINARY_DIR}/${manpage}.xml)
    set(outvar_html ${CMAKE_CURRENT_BINARY_DIR}/${manpage}.html)
    set(outvar_man  ${CMAKE_CURRENT_BINARY_DIR}/${manpage}.${section})

    add_custom_command(
        OUTPUT  ${outvar_xml}
        COMMAND ${asciidoc_exe}
                -b docbook -d manpage
                ${asciidoc_opts}
                -o ${outvar_xml}
                ${invar_txt}
        MAIN_DEPENDENCY
                ${invar_txt}
        WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running asciidoc for xml on \"${manpage}.txt\"")

    add_custom_command(
        OUTPUT  ${outvar_man}
        COMMAND ${xmlto_exe}
                ${xmlto_opts}
                man
                ${outvar_xml}
        MAIN_DEPENDENCY
                ${outvar_xml}
        WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating manpage for \"${manpage}.xml\"")

    add_custom_command(
        OUTPUT  ${outvar_html}
        COMMAND  ${asciidoc_exe}
                -b xhtml11 -d manpage
                ${asciidoc_opts}
                -o ${outvar_html}
                ${invar_txt}
        MAIN_DEPENDENCY
                ${invar_txt}
        WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running asciidoc for html on \"${manpage}.txt\"")

    add_custom_target(doc-man-${manpage}
        DEPENDS
            ${outvar_man})

    add_custom_target(doc-html-${manpage}
        DEPENDS
            ${outvar_html})

    add_dependencies(doc-man doc-man-${manpage})
    add_dependencies(doc-html doc-html-${manpage})

    install(
        FILES       ${outvar_man}
        DESTINATION ${MANPAGE_INSTALL_PATH}/man${section}/
        COMPONENT   documentation)

    install(
        FILES       ${outvar_html}
        DESTINATION ${DOCUMENTATION_INSTALL_PATH}/html/
        COMPONENT   documentation)
endmacro (asciidoc_doc)
