import React, {useState} from 'react';

import { useHistory, useLocation} from "react-router-dom";

// import profilContext from '../profilContext';

// import {profilContext} from '../../profilContext';

function Login(props) {

  // const {profilValue, updateProfil} = useContext(profilContext);

  const [username, setUsername] = useState("ngoc2012");

  let history = useHistory();
  let location = useLocation();

  let { from } = location.state || { from: { pathname: "/" } };

  const authenticate = () => {
    // updateProfil({...profilValue, user: username, status: ">> Bienvenue " + username})
    props.setStatus(">> Bienvenue " + username)
    history.replace(from);
  }

  return (
    <form
      onSubmit = {e => {
        e.preventDefault();
        authenticate();
      }}
    >
      <input value={username} onChange={e => setUsername(e.target.value)}></input>
      <button onClick={authenticate}>Login</button>
    </form>
  )
}

export default Login;