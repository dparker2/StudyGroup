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
  var $currGroups = array();
  var $recGroups = array();
  var $favGroups = array();
  var $numFavorite = 5;
  var $numRecent = 5;

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
  function getCurrGroups() {
    return $this->currGroups;
  }
  function getRecentGroups(){
    $recentGroups = array_slice($this->recGroups, 0, $this->numRecent);
    return $recentGroups;
  }
  function getFavoriteGroups() {
    return $this->favGroups;
  }

  function setName($name) {
    $this->username = $name;
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
  function setNumFavorite($num) {
    $this->numFavorite = $num;
  }
  function setNumRecent($num) {
    $this->numRecent = $num;
  }
  function setGroup($groupID) {
    array_unshift($this->currGroups, $groupID);
  }
  function setRecGroup($groupID) {
    array_unshift($this->recGroups, $groupID);
  }
  function setFavGroup($groupID) {
    if (count($groupID) == $this->numFavorite) {
      array_unshift($this->favGroups, $groupID);
      array_pop($this->favGroups);
    }
    else
      array_unshift($this->favGroups, $groupID);
  }

  function removeFavGroup($groupID) {
    for($i = 0; $i < count($this->favGroups); $i++) {
      if ($this->favGroups[$i] == $groupID)
        array_splice($this->favGroups, $i, 1);
    }
  }
  function removeGroup($groupID) {
    for($i = 0; $i < (count($this->currGroups)); $i++) {
      if($this->currGroups[$i] == $groupID)
        array_splice($this->currGroups, $i, 1);
    }
  }
  function clearGroups() {
    $this->recGroups = array();
    $this->currGroups = array();
    $this->favGroups = array();
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
        $this->numMembers--;
      }
    }
  }
  function setGroupID($groupname) {
      $this->groupID = $groupname;
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
