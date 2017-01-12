add_executable (put_file put_file.c)
target_link_libraries (put_file qiniu crypto m)

add_executable (put_file_abortable put_file_abortable.c)
target_link_libraries (put_file_abortable qiniu crypto m)

add_executable (gen_download_url_with_deadline gen_download_url_with_deadline.c)
target_link_libraries (gen_download_url_with_deadline qiniu crypto m)
