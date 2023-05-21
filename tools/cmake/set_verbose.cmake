include(join_strings)

# Sets a cache variable with a docstring joined from multiple arguments:
# set(<variable> <value>... CACHE <type> <docstring>...) This allows splitting a
# long docstring for readability.
function(set_verbose)
  # cmake_parse_arguments is broken in CMake 3.4 (cannot parse CACHE) so use
  # list instead.
  list(GET ARGN 0 var)
  list(REMOVE_AT ARGN 0)
  list(GET ARGN 0 val)
  list(REMOVE_AT ARGN 0)
  list(REMOVE_AT ARGN 0)
  list(GET ARGN 0 type)
  list(REMOVE_AT ARGN 0)
  join_strings(doc ${ARGN})
  set(${var}
      ${val}
      CACHE ${type} ${doc}
  )
endfunction()
