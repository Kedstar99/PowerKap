package EnergyModule.Action;

import com.intellij.openapi.module.Module;
import com.intellij.openapi.module.ModuleManager;
import com.intellij.openapi.project.Project;
import com.intellij.openapi.roots.ModuleRootManager;
import com.intellij.openapi.vfs.VirtualFile;
import com.intellij.psi.*;
import com.intellij.psi.codeStyle.JavaCodeStyleManager;
import com.intellij.util.IncorrectOperationException;

import java.util.StringTokenizer;

/**
 * Created by kd913 on 30/04/17.
 */
class   EnergyProfilerGenerator {
    static String className = "TimeKeeper.timeKeeper.TimeKeeper";
    static boolean isEnabled = false;

    EnergyProfilerGenerator() {
    }

    private String insertTimePointFunction() {
        return "public static void insertTimePoint(String filepath, String className, int lineNumber) {\n" + "callIndex++;\n" +
                "annotations.add(new EnergyAnnotation(new java.util.Date(), filepath, className, lineNumber, callIndex));" +
                "}\n";
    }

    private String printTimePointsFunction() {
        return "public static void printTimePoints() {\n" + "try {\n" +
                "java.io.Writer w = new java.io.FileWriter(\"elapsedtimes.txt\", true);\n" +
                "if (!annotations.isEmpty()) {\n" +
                "for (EnergyAnnotation annotation : annotations) {\n" +
                "w.write(annotation.toString());\n" +
                "}\n" +
                "w.close();\n" +
                "annotations.clear();\n" +
                "Thread.sleep(1000);\n" +
                "}\n" +
                "} catch (java.io.IOException | InterruptedException e ) {\n" +
                "e.printStackTrace();\n" +
                "}\n" +
                "}\n";
    }

    private String generateStaticFields() {
        return "private static java.util.List<EnergyAnnotation> annotations =  java.util.Collections.synchronizedList(new java.util.ArrayList());\n ";
    }

    private String generateStaticAnnotationConstructor() {
        return "EnergyAnnotation(java.util.Date date, String filePath, String className, int lineNumber, int callIndex) {\n" +
                "this.date = date;\n" +
                "this.className = className;\n" +
                "this.lineNumber = lineNumber;\n" +
                "this.callIndex = callIndex;\n" +
                "this.filePath = filePath;\n" +
                "}\n";
    }

    private String generateStaticAnnotationToString() {
        return "public String toString() {\n" +
                "java.text.SimpleDateFormat dateformat = new java.text.SimpleDateFormat(\"yyyy:MM:dd HH:mm:ss.SSS\");\n" +
                "return callIndex + \", \" + dateformat.format(date) + \", \" + filePath + \", \" + className + \", \" + lineNumber + \"\\n\";\n" +
                "}\n";
    }

    void generate(PsiClass timeKeeper, Project project) {
        PsiElementFactory elementFactory = JavaPsiFacade.getElementFactory(project);

        PsiField timeList = elementFactory.createFieldFromText(generateStaticFields(), timeKeeper);
        PsiField staticCallIndex = elementFactory.createFieldFromText("private static int callIndex = 0;\n", timeKeeper);
        PsiMethod insertFunction = elementFactory.createMethodFromText(insertTimePointFunction(), timeKeeper);
        PsiMethod printFunction = elementFactory.createMethodFromText(printTimePointsFunction(), timeKeeper);


        PsiClass psiClass = elementFactory.createClass("EnergyAnnotation");
        psiClass.getModifierList().setModifierProperty("public", false);
        psiClass.getModifierList().setModifierProperty("static", true);

        PsiField date = elementFactory.createFieldFromText("java.util.Date date;\n", psiClass);
        PsiField className = elementFactory.createFieldFromText("String className;\n", psiClass);
        PsiField lineNumber = elementFactory.createFieldFromText("int lineNumber;\n", psiClass);
        PsiField callIndex = elementFactory.createFieldFromText("int callIndex;\n", psiClass);
        PsiField filePath = elementFactory.createFieldFromText("String filePath;\n", psiClass);
        PsiMethod constructor = elementFactory.createMethodFromText(generateStaticAnnotationConstructor(), psiClass);
        PsiMethod toStringMethod = elementFactory.createMethodFromText(generateStaticAnnotationToString(), psiClass);
        JavaCodeStyleManager styleManager = JavaCodeStyleManager.getInstance(project);

        styleManager.shortenClassReferences(timeKeeper.addBefore(timeList, timeKeeper.getLastChild()));
        styleManager.shortenClassReferences(timeKeeper.addBefore(staticCallIndex, timeKeeper.getLastChild()));
        styleManager.shortenClassReferences(timeKeeper.addBefore(insertFunction, timeKeeper.getLastChild()));
        styleManager.shortenClassReferences(timeKeeper.addBefore(printFunction, timeKeeper.getLastChild()));

        styleManager.shortenClassReferences(psiClass.addBefore(date, psiClass.getLastChild()));
        styleManager.shortenClassReferences(psiClass.addBefore(className, psiClass.getLastChild()));
        styleManager.shortenClassReferences(psiClass.addBefore(lineNumber, psiClass.getLastChild()));
        styleManager.shortenClassReferences(psiClass.addBefore(callIndex, psiClass.getLastChild()));
        styleManager.shortenClassReferences(psiClass.addBefore(filePath, psiClass.getLastChild()));


        styleManager.shortenClassReferences(psiClass.addBefore(constructor, psiClass.getLastChild()));
        styleManager.shortenClassReferences(psiClass.addBefore(toStringMethod, psiClass.getLastChild()));
        styleManager.shortenClassReferences(timeKeeper.addBefore(psiClass, timeKeeper.getLastChild()));

    }

    static boolean isEnabled(Project project) {
        VirtualFile sourceDir = getModuleSource(project);
        boolean result = false;
        if (sourceDir != null) {
            PsiDirectory sourceDirectory = PsiManager.getInstance(project).findDirectory(sourceDir);
            if (sourceDirectory != null) {
                PsiDirectory outerPackage = sourceDirectory.findSubdirectory("TimeKeeper");
                if (outerPackage != null) {
                    PsiDirectory innerPackage = outerPackage.findSubdirectory("timeKeeper");
                    if (innerPackage != null) {
                        PsiFile javaFile = innerPackage.findFile("TimeKeeper.java");
                        if (javaFile instanceof PsiJavaFile) {
                            for (PsiClass elem: ((PsiJavaFile) javaFile).getClasses()) {
                                PsiMethod[] methods = elem.getMethods();
                                if (containsMethod(methods, "insertTimePoint") && containsMethod(methods, "printTimePoints")) {
                                    result = true;
                                }
                            }

                        }
                    }
                }
            }
        }
        return result;
    }

    private static boolean containsMethod(PsiMethod[] methods, String string) {
        for (PsiMethod m: methods) {
            if (m.toString().contains(string)) {
                return true;
            }
        }
        return false;
    }

    static VirtualFile getModuleSource(Project project) {
        ModuleManager manager = ModuleManager.getInstance(project);
        Module[] modules = manager.getModules();
        VirtualFile result = null;
        for (Module module : modules) {
            ModuleRootManager root = ModuleRootManager.getInstance(module);
            result =  root.getSourceRoots()[0];
        }
        return result;
    }

    PsiDirectory createDirectory(PsiDirectory parent, String name)
            throws IncorrectOperationException {
        PsiDirectory result = null;

        for (PsiDirectory dir : parent.getSubdirectories()) {
            if (dir.getName().equalsIgnoreCase(name)) {
                result = dir;
                break;
            }
        }

        if (null == result) {
            result = parent.createSubdirectory(name);
        }

        return result;
    }

    PsiDirectory createPackage(PsiDirectory sourceDir)
            throws IncorrectOperationException {
        PsiDirectory parent = sourceDir;
        StringTokenizer token = new StringTokenizer("timeKeeper", ".");
        while (token.hasMoreTokens()) {
            String dirName = token.nextToken();
            parent = createDirectory(parent, dirName);
        }
        return parent;
    }


}


