
function splitAttr(attr) {
  let attrList = []
  let attrArray = attr.split('')
  attrArray.forEach( (l, i) => {
    if (l === ".") {attrList.push({'type': 'attr', 'value': ""})}
    else if (l === "[") {
      if (attrArray[i+1] === "'") {attrList.push({'type': 'key', 'value': ""})}
      else {attrList.push({'type': 'index', 'value': ""})}
    } else {
      if (i === 0) {attrList.push({'type': 'attr', 'value': l})}    
      else {
        if (!(["'", "]"]).includes(l)) {attrList[attrList.length-1]['value'] += l}
      }
          
    }        
  })
  return attrList
}
    
function getAttrKeyLevel0(obj, attrList, level = 0) {
  if (attrList.length === level) {
    return obj;
  }
  let typ = attrList[0]['type']
  let val = attrList[0]['value']

  // The rest of attribute list
  let attrList0 = []
  attrList.forEach((v, i) => {
    if (i>0) {attrList0.push(v)}
  })

  if (typ === 'index') {
    if (val === 'i') {
      if (obj.length === undefined) {return null}
      return obj.map(o => getAttrKeyLevel0(o, attrList0))
    }
    else {
      if (obj.length < parseInt(val)) {return null}
      return getAttrKeyLevel0(obj[parseInt(val)], attrList0)
    }
  }

  if (!Object.keys(obj).includes(val)) {return null}
  if (typ === 'key') {return getAttrKeyLevel0(obj[val], attrList0)}
  if (typ === 'attr') {return getAttrKeyLevel0(obj[val], attrList0)}
      
}

function checkUnique(attr0) {
  let b = [];
  let paratheses = ["['", "']", "[", "]"];

  paratheses.forEach(a => {
    b.push((attr0.split(a)).length - 1)
  });
  
  if ((b[0] !== b[1]) | (b[2] !== b[3])) {
    console.log("Attribute text not correct " + attr0 + " (Core.attr)")
    return false;
  }

  return true;
}
    
export function getAttrKeyLevel(obj, attr) {
  
  // Check if attribute is correct
  if (!checkUnique(attr)) {return attr;}

  // Split attr
  // console.log(splitAttr(attr))
  // console.log(splitAttr(attr), getAttrKeyLevel0(obj, splitAttr(attr)))
  return getAttrKeyLevel0(obj, splitAttr(attr))
}

export function setAttrKeyLevel(obj, attr, value) {

  // Check if attribute is correct
  if (!checkUnique(attr)) {throw new Error();}

  let attrList = splitAttr(attr)
  if (attrList.length === 1) {
    obj[attrList[0].value] = value;
  } else {
    let obj0 = getAttrKeyLevel0(obj, attrList, 1);
    obj0[attrList[attrList.length-1].value] = value;
  }
}

// ========= Check with nodejs ==========
// > a = {"val": "initial"}
// { val: 'initial' }
// > function foo(a) {return a['val']}
// undefined
// > b = foo(a)
// 'initial'
// > b= 'changed'
// 'changed'
// > a
// { val: 'initial' }
// > function foo(a) {return a}
// undefined
// > b = foo(a)
// { val: 'initial' }
// > b['val']='changed'
// 'changed'
// > a
// { val: 'changed' }
// > a = {"val": {"val2": "initial"}}
// { val: { val2: 'initial' } }
// > function foo(a) {return a['val']}
// undefined
// > b = foo(a)
// { val2: 'initial' }
// > b['val2']='changed'
// 'changed'
// > a
// { val: { val2: 'changed' } }
// > function foo2(b) {b['val2'] = 'changed2'}
// undefined
// > foo2(b)
// undefined
// > a
// { val: { val2: 'changed2' } } 