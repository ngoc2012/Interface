import React from 'react';

import { useHistory, useLocation} from "react-router-dom";

// import profilContext from '../profilContext';

// import {profilContext} from '../../profilContext';

function Logout(props) {

  // const {profilValue, updateProfil} = useContext(profilContext);

  let history = useHistory();
  let location = useLocation();

  let { from } = location.state || { from: { pathname: "/" } };

  const logoutProcess = () => {
    // updateProfil({...profilValue, user: null, status: ">> Au revoir"})
    props.setStatus(">> Au revoir")
    history.replace(from);
  }
  return (
    <button onClick={logoutProcess}>Logout</button>
  )
}

export default Logout;