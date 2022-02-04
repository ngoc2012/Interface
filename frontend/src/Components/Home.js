import React from 'react';

// import React, {Suspense, lazy, useState} from 'react';
import { BrowserRouter as Router, Link} from "react-router-dom";

// import {profilContext} from '../profilContext';
// const Editor = lazy(() => import('./Components/Editor/Editor'))

function Home() {
  return <div>
    <div>Page de bienvenue</div>
    <Link to="/editor/defaults/BASecRecDimELU">BA</Link>
    <br/>
    <Link to="/rido/defaults/sectionBA">Rido</Link>
    <Router></Router>
  </div>
}

export default Home;