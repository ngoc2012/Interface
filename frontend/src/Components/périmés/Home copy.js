import React from 'react';

import { useHistory } from 'react-router-dom';

function Home() {

  const history = useHistory();

  const routeChange = () =>{
    history.push("/editor/defaults/sectionBA");
  }

  return <div>
    <div>Page de bienvenue</div>
    <button onClick={routeChange}>BA</button>
  </div>
}

export default Home;