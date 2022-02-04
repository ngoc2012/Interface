// "/Gentilly/IndE/RIDO" =>
export default function key2list(props) {
  
  let fileFolderList = []
  // console.log(props.files)
  props.files.forEach((element, i) => {
    let folders = element.key.split("/")
    folders.splice(0,1)
    // "/IndE/RIDO/CF.LST" => ["IndE", "RIDO", "CF.LST"]
    element.parent = []
    if (folders.length > 1) {element.parent = folders.slice(0, folders.length-1)}

    let lastMatch = 0
    if (i>0) {
      // "/Coulommiers/Ind2/tables/resultsTable.ods" => "/Gentilly/IndE/30.25.pdf"
      // "Coulommiers/Ind2/tables" => "Gentilly/IndE"
      // "Gentilly/IndA" => "Gentilly/IndE" => lastMatch = 1 => [IndE]
      while ((lastMatch <= (props.files[i-1].parent.length-1)) && (lastMatch <= (element.parent.length-1)) && (props.files[i-1].parent[lastMatch] === element.parent[lastMatch])) {lastMatch += 1}
    }
      for (let j = lastMatch; j < element.parent.length; j++) {
        fileFolderList.push({
          type: 'folder',
          level: j+1,
          collapsed: j+1 <= props.levelMax ? false : true,
          parent: element.parent.slice(0, j),
          name: element.parent[j],
          key: '/' + element.parent.slice(0, j+1).join('/'),
          // key: element.parent.slice(0, j+1)
        })
      }
    
    
    let fileWithExtension = folders.slice(-1)[0].split('.')
    fileFolderList.push({
      type: (fileWithExtension.length > 1) ? 'file' : 'folder',
      level: element.parent.length + 1,
      collapsed: element.parent.length + 1 <= props.levelMax ? false : true,
      parent: element.parent,
      name: folders.slice(-1)[0],
      key: element.key,
      modified: element.modified
    })
    
  });
  // console.log(fileFolderList)
  return fileFolderList;
}