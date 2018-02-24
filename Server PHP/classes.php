<?php

class User {
  function __construct() {
    echo "Constructing user class\n";
  }
  function __destruct() {
    echo "Destructing user class\n";
  }

  var $username;
  var $ip;
  var $socket;
  var $email;
  var $group = array();

  function getName() {
    return $this->username;
  }
  function getIP() {
    return $this->ip;
  }
  function getSocket() {
    return $this->socket;
  }
  function getEmail() {
    return $this->email;
  }
  function getGroup() {
    return $this->group;
  }
  function getRecentGroup(){
    $recentGroup = array_slice($this->group, 0, 5);
    return $recentGroup;
  }

  function setName($Name) {
    $this->username = $Name;
  }
  function setIP($ipAddress) {
    $this->ip = $ipAddress;
  }
  function setSocket($sock) {
    $this->socket = $sock;
  }
  function setEmail($usrEmail) {
    $this->email = $usrEmail;
  }
  function setGroup($groupID) {
    array_unshift($this->group, $groupID);
  }
  function removeGroup($groupID) {
    for($i = 0; $i < (count($this->group)); $i++) {
      if($this->group[$i] == $groupID)
        array_splice($this->group, $i, 1);
    }
  }

}

?>
