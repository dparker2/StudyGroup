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
  }

}

class Group {
  function _construct() {
    echo "Creating Group Class\n";
  }
  function __destruct() {
    echo "Destructing Group Class\n";
  }

  var $groupID;
  var $numMembers;
  var $admin;
  var $members = array();
  var $memberIPs = array();

  function getGroupID() {
    return $this->groupID;
  }
  function getNumMembers() {
    return $this->numMembers;
  }
  function getMembers() {
    return $this->members;
  }
  function getMemberIP() {
    return $this->memberIPs;
  }
  function getAdmin() {
    return $this->admin;
  }

  function removeMember($username) {
    for ($i = 0; $i < $this->numMembers; $i++) {
      if ($this->members[$i] == $username) {
        array_splice($this->members, $i, 1);
        array_splice($this->memberIPs, $i, 1);
      }
    }
    $this->numMembers--;
  }
  function setGroupID($name) {
      $this->groupID = $name;
  }
  function setNum() {
    $this->numMembers = count($this->members);
  }
  function setMember($member) {
    array_push($this->members, $member);
  }
  function setMemberIP($ipaddress) {
    array_push($this->memberIPs, $ipaddress);
  }
  function setAdmin($adminName) {
    $this->admin = $adminName;
  }
}

?>
