import React, {Suspense, lazy, useState} from 'react';
import { BrowserRouter as Router, Route, Link, Switch} from "react-router-dom";
// import Cookies from 'universal-cookie';

// https://reactjs.org/docs/code-splitting.html#route-based-code-splitting
const Home = lazy(() => import('./Components/Home'))
const Profil = lazy(() => import('./Components/Profil'))
const Login = lazy(() => import('./Components/Authentication/Login'))
const Logout = lazy(() => import('./Components/Authentication/Logout'))
const Browser = lazy(() => import('./Components/Browser/Browser'))
const Editor = lazy(() => import('./Components/Editor/Editor'))
// const Rido = lazy(() => import('./Components/Editor/Rido'))
const About = lazy(() => import('./Components/About'))
const Parser = lazy(() => import('./Components/Parser/Parser'))


// function getNow() {
//   const d = new Date();
//   return d.getHours() + ":" + d.getMinutes() + ":" + d.getSeconds()
// }

function App() { 

  const url = {
    frontend: "http://localhost:3001",
    backend: "http://localhost:8001",
    backendc: "http://localhost:9000"
  } 
  
  const [user, setUser] = useState("ngoc2012")
  
  // Pass props to Route components
  const BrowserUser = () => (<Browser url={url} user={user} showFiles={true} levelMax={1} setCurrent={()=>{}}/>)
  const LoginUser = () => (<Login url={url} user={user} setUser={setUser}></Login>)
  const EditorUser = () => (<Editor url={url} user={user}></Editor>)
  const ParserUser = () => (<Parser url={url} user={user}></Parser>)

  // By updating the Home route with exact, now it will be rendered only if it matches the full path.
  // If not all path start with '/' will redirect to Home.
  return (
    <Router>
      <div className="App">
        
        <div className="nav-bar no-print">
          <Link to="/">Home</Link> |
          {user !== null && (<Link to="/profil">{user} | </Link>)}
          {user === null && (<Link to="/login">Se connecter | </Link>)}
          {user !== null && (<Link to="/logout">Se déconnecter | </Link>)}
          <Link to="/browser">Fichiers | </Link>
          <Link to="/editor">Editeur | </Link>
          <Link to="/parser">Parser | </Link>
          <Link to="/about">About</Link>
        </div>

        <Suspense fallback={<div>Loading...</div>}>
          <Switch>
            <Route path="/" exact component={Home}></Route>
            <Route path="/profil" exact component={Profil}></Route>
            <Route path="/login" exact component={LoginUser}></Route>
            <Route path="/logout" exact component={Logout}></Route>
            <Route path="/browser/:user" exact component={BrowserUser}></Route>
            <Route path="/editor/:user/:fileName" component={EditorUser}></Route>
            <Route path="/parser" component={ParserUser}></Route>
            
            {/* <Route path="/rido/:user/:fileName" exact component={RidoUser}></Route> */}
            <Route path="/about" exact component={About}></Route>
          </Switch>
        </Suspense>
      </div>
    </Router>
  );
}

export default App;
