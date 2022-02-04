import React, {useContext} from 'react';
import { useHistory, useLocation} from "react-router-dom";
// import  { Redirect } from 'react-router-dom'

// import {profilContext} from '../profilContext';

function useCheckUser(username) {

  // const {profilValue, updateProfil} = useContext(profilContext);

  // let history = useHistory();
  // let location = useLocation();

  if (username === null) {
    // updateProfil({
    //   ...profilValue,
    //   status: ">> Merci de s'identifier!!!"
    // })
    // let { from } = location.state || { from: { pathname: "/login" } };
    // history.replace("/login");
    // this.props.history.push('/login')
    return false;
  }
  return true;
}

export default useCheckUser;