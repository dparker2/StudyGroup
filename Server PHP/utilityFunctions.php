<?php

//Test function for existence.
function checkExists($connection, $query) {
  if ($stmt = mysqli_prepare($connection, $query)) {
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
    return $exists;
  }
}

?>
